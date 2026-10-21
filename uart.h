/*
 * uart.h
 *
 * Created: 9/6/2026 7:02:31 PM
 *  Author: HP
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define uart_buffer_size 64

void uart_init(void);
void uart_sendchar(char data);
void uart_sendstring(const char *str);
uint8_t uart_available(void);
char uart_readchar(void);

#endif /* UART_H_ */