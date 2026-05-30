#ifndef DS3231_H
#define DS3231_H

#include "main.h"
#include <stdint.h>
#include <stdio.h>

#define DS3231_ADRRESS 0x68
#define DS3231_SECS_REG 0x00
#define DS3231_MINS_REG 0x01
#define DS3231_HOURS_REG 0x02
#define DS3231_DAY_REG 0x03
#define DS3231_DATE_REG 0x04
#define DS3231_MONTH_REG 0x05
#define DS3231_YEAR_REG 0x06

#define DS3231_SECS_A1M1 0x07
#define DS3231_MINS_A1M1 0x08
#define DS3231_HOURS_A1M1 0x09
#define DS3231_DAY_A1M1 0x0A

#define DS3231_CONTROL 0x0E
#define DS3231_STATUS 0x0F
#define DS3231_MSB_TEMP 0x11

typedef struct DS3231 {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t date;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} DS3231;



void DS3231_Init(void);
void DS3231_SetTime(DS3231 *time);
void DS3231_GetTime(DS3231 *time);
void DS3231_SetAlarm(DS3231 *time);
void DS3231_GetAlarm(DS3231 *time);


#endif
