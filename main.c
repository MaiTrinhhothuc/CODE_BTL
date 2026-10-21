/*
 * CODE_BTL.c
 *
 * Created: 9/3/2026 9:23:26 PM
 * Author : HP
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "twi.h"
#include "ds3231.h"
#include "uart.h"
#include "parser.h"
#include "led.h"

volatile uint8_t flag_500ms = 0;

//timer1 tao ngat moi 500ms
void timer1_init(void)
{
	// CTC mode
	TCCR1A = 0;

	TCCR1B = (1 << WGM12);

	// F_CPU = 8 MHz
	// Prescaler = 64
	// OCR1A = 1249 -> 10 ms

	OCR1A = 1249;

	// Cho phép ng?t Compare Match A
	TIMSK1 = (1 << OCIE1A);

	// Prescaler = 64
	TCCR1B |= (1 << CS11) | (1 << CS10);
}

//ngat moi 500ms
ISR(TIMER1_COMPA_vect)
{
	static uint8_t lcd_counter = 0;

	// Xu ly LED
	led_tick_10ms();

	// Tao su kien cap nhat LCD moi 500ms
	lcd_counter++;

	if(lcd_counter >= 50)
	{
		lcd_counter = 0;
		flag_500ms = 1;
	}
}

int main(void)
{
	RTC_TIME time;
	
	char date_str[16];
	char time_str[16];
	
	char old_date_str[16]="";
	char old_time_str[16]="";

	//khoi tao led
	led_init();
	
	//khoi tao TWI
	twi_init();
	
	//khoi tao Timer1
	timer1_init();
	
	//khoi tao LCD
	LCD_INIT();
	
	//khoi tao uart
	uart_init();
	
	//khoi tao parser
	parser_init();
	
	//doc RTC lan dau
	ds3231_read(&time);
	
	//cho parser su dung bien time
	parser_setrtc(&time);
	
	//cho phep ngat toan cuc
	sei();
	
	while(1){
		
		//xu ly uart
		while(uart_available()){
			char data;
			data=uart_readchar();
			parser_processchar(data);
		}
		
		//cap nhat rtc+lcd
		if(flag_500ms)
		{
			flag_500ms=0;
			
			//doc tgian ds3231
			ds3231_read(&time);
			
			make_date_string(&time, date_str);
			make_time_string(&time, time_str);

			// Cap nhat trang thai LED tren LCD
			if(led_get_mode() == on)
			{
				time_str[11] = 'O';
				time_str[12] = 'N';
				time_str[13] = ' ';
			}
			else if(led_get_mode() == off)
			{
				time_str[11] = 'O';
				time_str[12] = 'F';
				time_str[13] = 'F';
			}
			else
			{
				time_str[11] = 'B';
				time_str[12] = 'L';
				time_str[13] = 'K';
			}

			LCD_UpdateLine(0,0,date_str,old_date_str);
			LCD_UpdateLine(1,0,time_str,old_time_str);
		}
	}
}