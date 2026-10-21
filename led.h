/*
 * led.h
 *
 * Created: 9/7/2026 6:49:31 PM
 *  Author: HP
 */ 


#ifndef LED_H_
#define LED_H_

#include <stdint.h>

#define off   0
#define on    1
#define blink 2

void led_init(void);

void led_on(void);
void led_off(void);
void led_blink(uint16_t period);

uint8_t led_get_mode(void);
uint16_t led_get_period(void);

//goi moi 10ms tu Timer1
void led_tick_10ms(void);

#endif /* LED_H_ */