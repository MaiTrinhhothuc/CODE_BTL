/*
 * parser.h
 *
 * Created: 9/7/2026 3:29:04 PM
 *  Author: HP
 */ 


#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include "ds3231.h"
#include "led.h"

#define COMMAND_BUFFER_SIZE 49

void parser_init(void);

void parser_processchar(char data);

void parser_processcommand(void);
uint8_t parser_time_command(char *cmd, uint8_t *hour, uint8_t *minute, uint8_t *second);
uint8_t parser_date_command(char *cmd, uint8_t *date, uint8_t *month, uint8_t *year, uint8_t *day);

uint8_t parser_leap_year(uint16_t year);
uint8_t parser_valid_date(uint8_t date, uint8_t month, uint16_t year);

void parser_setrtc(RTC_TIME *time);
#endif /* PARSER_H_ */