/*
 * lcd.c
 *
 * Created: 9/6/2026 4:50:29 PM
 *  Author: HP
 */ 
#define	F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "lcd.h"



void delay_ms(unsigned int n){
	while(n--){
		_delay_ms(1);
	}
}

//HAM GHI 4 BIT RA LCD
//LCD D4-D7 NOI VOI PA4-PA7

void OUT_LCD4(unsigned char val){
	lcd=(lcd&0x0F)|(val&0xF0);
	lcd|=(1<<E);	//SBI LCD,E
	lcd&=~(1<<E);	//CBI LCD,E
}

void OUT_LCD4_2(unsigned char chr_val){
	unsigned char chr1,chr2;
	delay_ms(1);
	
	//lay trang thai RS
	chr1=lcd&(1<<RS);
	
	//lay 4 bit cao
	chr2=chr_val&0xf0;
	
	//ghep bit RS
	chr2|=chr1;
	
	OUT_LCD4(chr2);
	
	delay_ms(1);
	
	//dua 4 bit thap len 4 bit cao
	chr2=chr_val<<4;
	chr2&=0xF0;
	
	//ghep RS
	chr2|=chr1;
	
	OUT_LCD4(chr2);
}

void LCD_INIT(void){
	lcd_dr|=(1<<RS)|(1<<RW)|(1<<E)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7);
	
	lcd&=~(1<<RS);		//truy xuat lenh RS=0; CBI LCD,RS ghi lenh
	lcd&=~(1<<RW);		//RW=0
	lcd&=~(1<<E);		//EN=0
	
	delay_ms(20);
	
	//0x30 lan 1
	OUT_LCD4(0x30);
	delay_ms(5);
	
	//0x30 lan 2
	OUT_LCD4(0x30);
	delay_ms(1);
	
	//Chuyen sang 4 bit
	OUT_LCD4(0x20);
	delay_ms(1);
	
	OUT_LCD4_2(0x28);	//ma lenh function set
	OUT_LCD4_2(0x01);	//ma lenh clear display
	delay_ms(2);
	OUT_LCD4_2(0x0C);	//ma lenh display control on/off	
	OUT_LCD4_2(0X06);	//ma lenh entry mode set
}

//DAT CON TRO LCD
//row = 0 -> dong 1
//row = 1 -> dong 2
void LCD_SetCursor(unsigned char row, unsigned char col){
	unsigned char address;
	if(row==0) address=0x00+col;
	else address=0x40 + col;
	lcd&=~(1<<RS);
	
	//Command: Set DDRAM address	
	OUT_LCD4_2(0x80|address);
}

//HIEN THI 1 KY TU
void LCD_PutChar(char c){
	lcd|=(1<<RS);	//ghi data
	OUT_LCD4_2(c);
}

//HIEN THI CHUOI
void LCD_Print(char *str){
	while(*str){
		LCD_PutChar(*str);
		str++;
	}
}

//HIEN THI SO 2 CHU SO: 5 -> 05
void LCD_Print2Digit(uint8_t number){
	LCD_PutChar((number/10)+'0');
	LCD_PutChar((number%10)+'0');
}

void LCD_UpdateLine(unsigned char row, unsigned char col, char *new_str, char *old_str){
	unsigned char i;
	for(i = 0; new_str[i] != '\0'; i++)
	{
		if(new_str[i] != old_str[i])
		{
			LCD_SetCursor(row, col + i);
			LCD_PutChar(new_str[i]);

			old_str[i] = new_str[i];
		}
	}
}