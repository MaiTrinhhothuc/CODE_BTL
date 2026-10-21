/*
 * twi.c
 *
 * Created: 9/6/2026 6:04:11 PM
 *  Author: HP
 */
#define	F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>

#include "twi.h"

/*KHOI TAO TWI:
F_CPU=8MHz
TWPS=1->PRESCALER=4
TWBR=8
F_SCL=F_CPU/(16+2*TWBR*4^TWWPS)=8MHz/(16+2*8*4)=100kHz
*/

void twi_init(){
	TWBR=8;	//BIT RATE: HE SO CHIA DIEU KHIEN TAN SO SCL
	TWSR=1;	//STATUS REGISTER: CHUA THANH GHI TWPS, F(SCL)=100KHz
	TWCR=(1<<TWEN);	//ENABLE BIT: CHO PHEP GIAO TIEP TWI
}

 //xung start
void twi_start(){
	TWCR=(1<<TWEN)|(1<<TWSTA)|(1<<TWINT);	//PHAT XUNG START
	while(!(TWCR&(1<<TWINT)));	//CHO CO TWINT=1
}

//xung stop
void twi_stop(void){
	TWCR=(1<<TWEN)|(1<<TWSTO)|(1<<TWINT);	//PHAT XUNG STOP
}

//gui 1 byte
void twi_write(uint8_t data){
	TWDR= data;
	TWCR=(1<<TWEN)|(1<<TWINT);	//GHI ADDR/DATA RA TWI
	while(!(TWCR&(1<<TWINT)));	//CHO CO TWINT=1
}

//doc 1 byte + gui ack
uint8_t twi_read_ack(void){
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR&(1<<TWINT)));
	return TWDR;
}

//doc 1 byte + gui nack
uint8_t twi_read_nack(void){
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	return TWDR; // ???
}