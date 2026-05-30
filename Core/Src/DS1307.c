#include <stdio.h>
#include "DS1307.h"
#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
void DS1307_Init(void ) {
     
    
}
void DS1307_GetTime(DS1307 *time){
    
}
void DS1307_SetTime(DS1307 *time) {
    uint8_t time_data[7];
    time_data[0] = time->seconds;
    time_data[1] = time->minutes;
    time_data[2] = time->hours;
    time_data[3] = time->date;
    time_data[4] = time->day;   
    time_data[5] = time->month;
    time_data[6] = time->year ;
    HAL_I2C_Mem_Write(&hi2c1, DS1307_ADRRESS << 1, DS1307_SECS_REG, I2C_MEMADD_SIZE_8BIT, time_data, 7, 100); 


}


