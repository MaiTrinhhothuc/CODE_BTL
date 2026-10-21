/*
 * twi.h
 *
 * Created: 9/6/2026 6:04:03 PM
 *  Author: HP
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>

void twi_init(void);
void twi_start(void);
void twi_stop(void);
void twi_write(uint8_t data);
uint8_t twi_read_ack(void);
uint8_t twi_read_nack(void);



#endif /* TWI_H_ */