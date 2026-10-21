/*
 * led.c
 *
 * Created: 9/7/2026 6:49:39 PM
 *  Author: HP
 */ 
/*
 * led.c
 *
 * LED control
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>

#include "led.h"

#define led_pin PB0

volatile uint8_t led_mode = off;
volatile uint16_t led_period = 0;
volatile uint16_t led_counter = 0;

void led_init(void)
{
	DDRB |= (1 << led_pin);
	PORTB &= ~(1 << led_pin);

	led_mode = off;
	led_period = 0;
	led_counter = 0;
}

void led_on(void)
{
	led_mode = on;
	led_counter = 0;

	PORTB |= (1 << led_pin);
}

void led_off(void)
{
	led_mode = off;
	led_counter = 0;

	PORTB &= ~(1 << led_pin);
}

void led_blink(uint16_t period)
{
	led_mode = blink;
	led_period = period;
	led_counter = 0;

	/* Bat LED ngay khi bat dau BLINK */
	PORTB |= (1 << led_pin);
}

uint8_t led_get_mode(void)
{
	return led_mode;
}

uint16_t led_get_period(void)
{
	return led_period;
}

void led_tick_10ms(void)
{
	if(led_mode == blink)
	{
		led_counter++;

		/*
		 * period la chu ky toan bo
		 * LED doi trang thai moi T/2
		 *
		 * Timer tick moi 10 ms
		 */
		if(led_counter >= (led_period / 20))
		{
			PORTB ^= (1 << led_pin);
			led_counter = 0;
		}
	}
	else if(led_mode == on)
	{
		PORTB |= (1 << led_pin);
		led_counter = 0;
	}
	else
	{
		PORTB &= ~(1 << led_pin);
		led_counter = 0;
	}
}