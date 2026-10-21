/*
 * uart.c
 *
 * Created: 9/6/2026 7:02:41 PM
 *  Author: HP
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uart.h"

/* ring buffer */
volatile char uart_buffer[uart_buffer_size];

volatile uint8_t uart_rx_head=0;
volatile uint8_t uart_rx_tail=0;

void uart_init(void){
	//baud rate = 9600 F_CPU = 8MHz Normal mode
	UBRR0H=0;
	UBRR0L=51;
	
	//cho phep TX RX
	UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	
	//8 bit data, 1 stop bit, no parity
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
}

//gui 1 ky tu
void uart_sendchar(char data){
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0=data;
}

//giu chuoi
void uart_sendstring(const char *str){
	while(*str!='\0'){
		uart_sendchar(*str);
		str++;
	}
}

//ktra buffer co du lieu hay khong
uint8_t uart_available(void){
	if(uart_rx_head!=uart_rx_tail){
		return 1;
	}
	return 0;
}

//doc 1 ky tu tu ring buffer
char uart_readchar(void){
	char data;
	
	if(uart_rx_head==uart_rx_tail){
		return 0;
	}
	data=uart_buffer[uart_rx_tail];
	uart_rx_tail++;
	
	if(uart_rx_tail>=uart_buffer_size){
		uart_rx_tail=0;
	}
	return data;
}

//uart rx interrupt
ISR(USART0_RX_vect){
	char data;
	uint8_t next_head;
	data=UDR0;
	
	next_head=uart_rx_head+1;
	
	next_head=uart_rx_head+1;
	if(next_head>=uart_buffer_size){
		next_head=0;
	}
	
	//neu buffer chua day thi luu du lieu
	if(next_head!=uart_rx_tail){
		uart_buffer[uart_rx_head]=data;
		uart_rx_head=next_head;
	}
}