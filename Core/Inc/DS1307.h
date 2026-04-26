#include <stdint.h>
#include <stdio.h>
#include "main.h"

#define  DS1307_ADRRESS 	0x68
#define  DS1307_SECS_REG  0x00
#define  DS1307_MINS_REG  0x01
#define  DS1307_HOURS_REG 0x02
#define  DS1307_DAY_REG 	0x03
#define  DS1307_DATE_REG 	0x04
#define  DS1307_MONTH_REG 0x05
#define  DS1307_YEAR_REG  0x06

#define  DS1307_SECS_A1M1 0x07
#define  DS1307_MINS_A1M1 0x08
#define  DS1307_HOURS_A1M1 0x09
#define  DS1307_DAY_A1M1 	0x0A

#define DS1307_CONTROL 0x0E
#define DS1307_STATUS 0x0F
#define DS1307_MSB_TEMP 0x11
typedef struct DS1307 {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t date;
    uint8_t day;
    uint8_t month;
    uint16_t year;
}DS1307;

void DS1307_Init(void );
void DS1307_SetTime(DS1307 *time);
void DS1307_GetTime(DS1307 *time);
void DS1307_SetAlarm(DS1307 *time);
void DS1307_GetAlarm(DS1307 *time);