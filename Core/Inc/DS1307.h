#include <stdint.h>
#define  DS1307_ADRRESS 0x68
#define  DS1307_SECS_REG 0x00
#define  DS1307_MINS_REG 0x01
#define  DS1307_HOURS_REG 0x02
#define  DS1307_DAY_REG 0x03
#define  DS1307_MONTH_REG 0x04

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
void DS1307_SetDate(DS1307 *date);
void DS1307_GetDate(DS1307 *date);  
