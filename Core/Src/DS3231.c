#include "DS3231.h"
#define DS3231_I2C_ADDR (DS3231_ADRRESS << 1)
extern I2C_HandleTypeDef hi2c1;
uint8_t DS3231_LastWriteYear = 0;
uint8_t DS3231_LastReadYear = 0;
HAL_StatusTypeDef DS3231_LastWriteStatus = HAL_ERROR;
HAL_StatusTypeDef DS3231_LastReadStatus = HAL_ERROR;
void DS3231_Init(void ) {
     
    
}
static uint8_t DecToBcd(uint8_t val) {
    return (uint8_t)((val / 10 << 4) | (val % 10));
}


static uint8_t BcdToDec(uint8_t val) {
    return ((val >> 4) * 10 + (val & 0x0F));
}

void DS3231_SetTime(DS3231 *time) {
    uint8_t data[7];
    data[0] = DecToBcd(time->seconds);
    data[1] = DecToBcd(time->minutes);
    data[2] = DecToBcd(time->hours) & 0x3F; // Đảm bảo bit 6 (chế độ 12h/24h) được tắt
    data[3] = DecToBcd(time->day);
    data[4] = DecToBcd(time->date);
    data[5] = DecToBcd(time->month);
    data[6] = DecToBcd(time->year);
    DS3231_LastWriteYear = data[6];

HAL_I2C_Mem_Write(&hi2c1, DS3231_I2C_ADDR, DS3231_SECS_REG, I2C_MEMADD_SIZE_8BIT, data, 7, 100);

}

void DS3231_GetTime(DS3231 *time) {
    uint8_t data[7];
    HAL_I2C_Mem_Read(&hi2c1, DS3231_I2C_ADDR, DS3231_SECS_REG, I2C_MEMADD_SIZE_8BIT, data, 7, 100);


    time->seconds = BcdToDec(data[0] & 0x7F); // Mask bit CH ở thanh ghi giây
    time->minutes = BcdToDec(data[1]);
    time->hours   = BcdToDec(data[2] & 0x3F); // Mask chế độ 12h/24h
    time->day     = BcdToDec(data[3]);
    time->date    = BcdToDec(data[4]);
    time->month   = BcdToDec(data[5]);
    time->year    = BcdToDec(data[6]);


}

void DS3231_SetAlarm(DS3231 *time){
		uint8_t data[4];
		data[0] = DecToBcd(time->seconds);
		data[1] = DecToBcd(time->minutes);
		data[2] = DecToBcd(time->hours) & 0x3F;
		data[3] = 0x80;
		
		HAL_I2C_Mem_Write(&hi2c1, DS3231_I2C_ADDR, DS3231_SECS_A1M1, I2C_MEMADD_SIZE_8BIT, data, 4, 100);


}
	
void DS3231_GetAlarm(DS3231 *time){
		uint8_t buffer[4];
		HAL_I2C_Mem_Read(&hi2c1, DS3231_I2C_ADDR, DS3231_SECS_A1M1, I2C_MEMADD_SIZE_8BIT, buffer, 4, 100);
		time->seconds = BcdToDec(buffer[0] & 0x7F);
    time->minutes = BcdToDec(buffer[1] & 0x7F);
	    time->hours   = BcdToDec(buffer[2] & 0x7F);

}

// Write single year register (two-digit year)
uint8_t DS3231_SetYear(uint8_t year) {
    uint8_t y = DecToBcd(year);
    DS3231_LastWriteYear = y;
    uint8_t attempts = 3;
    uint8_t rb = 0;
    for (uint8_t i = 0; i < attempts; ++i) {
        DS3231_LastWriteStatus = HAL_I2C_Mem_Write(&hi2c1, DS3231_I2C_ADDR, DS3231_YEAR_REG, I2C_MEMADD_SIZE_8BIT, &y, 1, 100);
        HAL_Delay(50);
        DS3231_LastReadStatus = HAL_I2C_Mem_Read(&hi2c1, DS3231_I2C_ADDR, DS3231_YEAR_REG, I2C_MEMADD_SIZE_8BIT, &rb, 1, 100);
        if (DS3231_LastReadStatus == HAL_OK) {
            DS3231_LastReadYear = rb;
            if (rb == y) return 1; // success
        }
        HAL_Delay(50);
    }
    // final values set for diagnostics
    DS3231_LastReadYear = rb;
    return 0; // failed after retries
}

// Force-write year register using raw I2C transmit/receive (fallback)
uint8_t DS3231_ForceWriteYear(uint8_t year) {
    uint8_t y = DecToBcd(year);
    uint8_t buf[2] = { DS3231_YEAR_REG, y };
    // Use Master Transmit (no mem add) to write register pointer then value
    DS3231_LastWriteStatus = HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_ADDR, buf, 2, 100);
    HAL_Delay(50);
    uint8_t rb = 0;
    DS3231_LastReadStatus = HAL_I2C_Master_Receive(&hi2c1, DS3231_I2C_ADDR, &rb, 1, 100);
    if (DS3231_LastReadStatus == HAL_OK) {
        DS3231_LastReadYear = rb;
        if (rb == y) return 1;
    }
    return 0;
}