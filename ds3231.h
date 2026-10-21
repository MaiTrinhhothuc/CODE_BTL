/*
 * ds3231.h
 *
 * Created: 9/6/2026 6:14:11 PM
 *  Author: HP
 */ 


#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>

//Dia chi TWI cua ds3231
#define DS3231_ADDR 0x68

//Cau truc luu tgian
typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	
	uint8_t day;	//thu trong tuan
	uint8_t date;	//ngay trong thang
	uint8_t month;
	uint8_t year;
} RTC_TIME;

//Khai bao ham
//Doc tgian tu ds3231
void ds3231_read(RTC_TIME *time);

//ghi gio vao ds3231
uint8_t ds3231_set_time(uint8_t hour, uint8_t minute, uint8_t second);

//ghi ngay vao ds3231
uint8_t ds3231_set_date(uint8_t date, uint8_t month, uint8_t year, uint8_t day);

//ghi ngay + gio
uint8_t ds3231_set_datetime(uint8_t date, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second, uint8_t day);

//doi DOW thanh ten thu
const char* ds3231_getdayname(uint8_t day);

//tao chuoi ngay thang de hien thi LCD
void make_date_string(RTC_TIME *time, char *str);

//tao chuoi tgian de hien thi LCD
void make_time_string(RTC_TIME *time, char *str);

//doc nhiet do
uint8_t ds3231_read_temperature(int16_t *temperature);

#endif /* DS3231_H_ */