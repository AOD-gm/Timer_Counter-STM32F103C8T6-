#ifndef DS1307_H
#define DS1307_H

/* Compatibility wrapper: map legacy DS1307 API to DS3231 implementation.
   This lets existing code that includes "DS1307.h" keep working while
   the real driver is implemented in DS3231.{h,c}.
*/

#include "DS3231.h"

/* Register/address compatibility macros */
#define DS1307_ADRRESS      DS3231_ADRRESS
#define DS1307_SECS_REG     DS3231_SECS_REG
#define DS1307_MINS_REG     DS3231_MINS_REG
#define DS1307_HOURS_REG    DS3231_HOURS_REG
#define DS1307_DAY_REG      DS3231_DAY_REG
#define DS1307_DATE_REG     DS3231_DATE_REG
#define DS1307_MONTH_REG    DS3231_MONTH_REG
#define DS1307_YEAR_REG     DS3231_YEAR_REG

#define DS1307_SECS_A1M1    DS3231_SECS_A1M1
#define DS1307_MINS_A1M1    DS3231_MINS_A1M1
#define DS1307_HOURS_A1M1   DS3231_HOURS_A1M1
#define DS1307_DAY_A1M1     DS3231_DAY_A1M1

#define DS1307_CONTROL      DS3231_CONTROL
#define DS1307_STATUS       DS3231_STATUS
#define DS1307_MSB_TEMP     DS3231_MSB_TEMP

/* Type alias so existing code using `DS1307` struct continues to compile */
typedef DS3231 DS1307;

/* Function alias macros mapping old API names to new implementation */
#define DS1307_Init        DS3231_Init
#define DS1307_SetTime     DS3231_SetTime
#define DS1307_GetTime     DS3231_GetTime
#define DS1307_SetAlarm    DS3231_SetAlarm
#define DS1307_GetAlarm    DS3231_GetAlarm

#endif
