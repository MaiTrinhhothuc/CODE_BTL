/*
 * ds3231.c
 *
 * Created: 9/6/2026 6:14:00 PM
 *  Author: HP
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>

#include "twi.h"
#include "ds3231.h"
#include "uart.h"

//CHUYEN BCD -> DECIMAL
uint8_t BCD_TO_DEC(uint8_t bcd){
	return ((bcd >> 4)*10) + (bcd & 0x0F);
}

//CHUYEN DEC -> BCD
uint8_t DEC_TO_BCD(uint8_t dec){
	return((dec/10)<<4)|(dec%10);
}

//ham in ma HEX
static char hex_digit(uint8_t value)
{
	if(value < 10)
	return '0' + value;
	else
	return 'A' + (value - 10);
}

void ds3231_read(RTC_TIME *time){
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	
	//START
	twi_start();
	
	//ghi dia chi ds3231 de giao tiep + bit 0 (write)
	twi_write((DS3231_ADDR<<1)|0);	//7 bit dia chi + bit R/W
	
	//chon thanh ghi second (0x00)
	twi_write(0x00);
	
	//repeated start
	twi_start();
	
	//ghi dia chi ds3231 + bit 1 (read)
	twi_write((DS3231_ADDR<<1)|1);
	
	//doc 7 thanh ghi lien tiep
	sec = twi_read_ack();
	min = twi_read_ack();
	hour = twi_read_ack();
	day = twi_read_ack();
	date = twi_read_ack();
	month = twi_read_ack();
	year = twi_read_nack();
	
	//STOP
	twi_stop();
	
	//chuyen BCD sang Decimal
	time->second = BCD_TO_DEC(sec);
	time->minute = BCD_TO_DEC(min);
	time->hour = BCD_TO_DEC(hour & 0x3F);
	
	time->day = BCD_TO_DEC(day);
	time->date = BCD_TO_DEC(date&0x3F);
	time->month = BCD_TO_DEC(month&0x1F);
	time->year = BCD_TO_DEC(year);
}

const char* ds3231_getdayname(uint8_t day)
{
	switch(day)
	{
		case 1: return "SUN";
		case 2: return "MON";
		case 3: return "TUE";
		case 4: return "WED";
		case 5: return "THU";
		case 6: return "FRI";
		case 7: return "SAT";

		default: return "---";
	}
}

void make_date_string(RTC_TIME *time, char *str)
{
	const char *day_name;
	day_name = ds3231_getdayname(time->day);
	
	str[0] = (time->date / 10) + '0';
	str[1] = (time->date % 10) + '0';
	str[2] = '/';
	
	str[3] = (time->month / 10) + '0';
	str[4] = (time->month % 10) + '0';
	str[5] = '/';
	
	str[6] = '2';
	str[7] = '0';
	
	str[8] = (time->year / 10) + '0';
	str[9] = (time->year % 10) + '0';
	
	str[10] = ' ';
	
	str[11] = day_name[0];
	str[12] = day_name[1];
	str[13] = day_name[2];
	
	str[14] = ' ';
	str[15] = '\0';
}

void make_time_string(RTC_TIME *time, char *str)
{
	str[0] = (time->hour / 10) + '0';
	str[1] = (time->hour % 10) + '0';
	str[2] = ':';

	str[3] = (time->minute / 10) + '0';
	str[4] = (time->minute % 10) + '0';
	str[5] = ':';

	str[6] = (time->second / 10) + '0';
	str[7] = (time->second % 10) + '0';

	str[8] = ' ';
	
	// Tam thoi mac dinh
	str[9]  = 'L';
	str[10] = ':';
	str[11] = 'O';
	str[12] = 'F';
	str[13] = 'F';

	str[14] = ' ';
	str[15] = '\0';
}

uint8_t ds3231_set_time(uint8_t hour, uint8_t minute, uint8_t second)
{
	//ktra gia tri
	if(hour>23){
		return 0;
	}
	if(minute>59){
		return 0;
	}
	if(second>59){
		return 0;
	}
	
	//START
	twi_start();
	
	//ds3231 + write
	twi_write((DS3231_ADDR<<1)|0);
	
	//bat dau tu thanh ghi second - 0x00
	twi_write(0x00);
	
	//ghi second
	twi_write(DEC_TO_BCD(second));
	
	//ghi minute
	twi_write(DEC_TO_BCD(minute));
	
	//ghi hour - 24 hour mode
	twi_write(DEC_TO_BCD(hour));
	
	//STOP
	twi_stop();
	
	return 1;
}

uint8_t ds3231_set_date(uint8_t date, uint8_t month, uint8_t year, uint8_t day){
	//ktra range
	if(date<1||date>31){
		return 0;
	}
	if(month<1||month>12){
		return 0;
	}
	if(year>99){
		return 0;
	}
	if(day<1||day>7){
		return 0;
	}

	//ghi day-0x03
	//START
	twi_start();
	
	//ds3231 + write
	twi_write((DS3231_ADDR<<1)|0);
	
	//bat dau tu thanh ghi day - 0x03
	twi_write(0x03);
	
	//ghi day
	twi_write(DEC_TO_BCD(day));
	
	//STOP
	twi_stop();
	
	//repeated start de ghi date-month-year
	//START
	twi_start();
	
	//ds3231 + write
	twi_write((DS3231_ADDR<<1)|0);
	
	//bat dau tu thanh ghi date - 0x04
	twi_write(0x04);
	
	twi_write(DEC_TO_BCD(date));
	
	//ghi month
	twi_write(DEC_TO_BCD(month));
	
	//ghi year
	twi_write(DEC_TO_BCD(year));
	
	//STOP
	twi_stop();
	
	return 1;	
}

uint8_t ds3231_set_datetime(uint8_t date, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second, uint8_t day)
{
	twi_start();

	twi_write((DS3231_ADDR << 1) | 0);

	// Bat dau tu thanh ghi 0x00
	twi_write(0x00);

	twi_write(DEC_TO_BCD(second));
	twi_write(DEC_TO_BCD(minute));
	twi_write(DEC_TO_BCD(hour));

	twi_write(DEC_TO_BCD(day));
	twi_write(DEC_TO_BCD(date));
	twi_write(DEC_TO_BCD(month));
	twi_write(DEC_TO_BCD(year));

	twi_stop();

	return 1;
}

uint8_t ds3231_read_temperature(int16_t *temperature)
{
	uint8_t msb;
	uint8_t lsb;
	int16_t raw;

	// Doc thanh ghi nhiet do
	twi_start();
	twi_write((DS3231_ADDR << 1) | 0);
	twi_write(0x11);

	twi_start();
	twi_write((DS3231_ADDR << 1) | 1);

	msb = twi_read_ack();
	lsb = twi_read_nack();

	twi_stop();

	// Ghe?p thanh 10-bit signed
	raw = ((int16_t)((int8_t)msb) << 2) | (lsb >> 6);

	// Moi don vi raw = 0.25 do C
	*temperature = raw * 25;
	
	uart_sendstring("MSB=0x");
	uart_sendchar(hex_digit(msb >> 4));
	uart_sendchar(hex_digit(msb & 0x0F));

	uart_sendstring(" LSB=0x");
	uart_sendchar(hex_digit(lsb >> 4));
	uart_sendchar(hex_digit(lsb & 0x0F));

	uart_sendstring("\r\n");
	
	return 1;
}