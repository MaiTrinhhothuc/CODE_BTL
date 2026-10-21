/*
 * lcd.h
 *
 * Created: 9/6/2026 4:52:45 PM
 *  Author: HP
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stdint.h>

// KHAI BAO PORT LCD
#define lcd	PORTA
#define lcd_dr		DDRA

//KHAI BAO CHAN LCD
#define RS		PA0
#define RW		PA1
#define E		PA2

#define LCD_D4		PA4
#define LCD_D5		PA5
#define LCD_D6		PA6
#define LCD_D7		PA7

//CAC HAM LCD
void OUT_LCD4(unsigned char val);
void OUT_LCD4_2(unsigned char chr_val);

void LCD_INIT(void);
void LCD_SetCursor(unsigned char row, unsigned char col);
void LCD_PutChar(char c);
void LCD_Print(char *str);
void LCD_Print2Digit(uint8_t number);
void delay_ms(unsigned int n);

//chinh sua nhung so thay doi
void LCD_UpdateLine(unsigned char row, unsigned char col, char *new_str, char *old_str);
#endif /* LCD_H_ */