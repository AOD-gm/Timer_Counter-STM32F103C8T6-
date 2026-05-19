
#include "DS1307.h"
#define DS1307_I2C_ADDR (DS1307_ADRRESS << 1)
extern I2C_HandleTypeDef hi2c1;
void DS1307_Init(void ) {
     
    
}
static uint8_t DecToBcd(uint8_t val) {
    return (uint8_t)((val / 10 << 4) | (val % 10));
}


static uint8_t BcdToDec(uint8_t val) {
    return (uint8_t)((val >> 4) * 10 + (val & 0x0F));
}

void DS1307_SetTime(DS1307 *time) {
    uint8_t data[7];
    data[0] = DecToBcd(time->seconds);
    data[1] = DecToBcd(time->minutes);
    data[2] = DecToBcd(time->hours) & 0x3F; // Đảm bảo bit 6 (chế độ 12h/24h) được tắt
    data[3] = DecToBcd(time->day);
    data[4] = DecToBcd(time->date);
    data[5] = DecToBcd(time->month);
    data[6] = DecToBcd(time->year);

    HAL_I2C_Mem_Write(&hi2c1,DS1307_I2C_ADDR, DS1307_SECS_REG, I2C_MEMADD_SIZE_8BIT, data, 7, 100);
}

void DS1307_GetTime(DS1307 *time) {
    uint8_t data[7];
    HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDR, DS1307_SECS_REG, I2C_MEMADD_SIZE_8BIT, data, 7, 100);

    time->seconds = BcdToDec(data[0] & 0x7F); // Mask bit CH ở thanh ghi giây
    time->minutes = BcdToDec(data[1]);
    time->hours   = BcdToDec(data[2] & 0x3F); // Mask chế độ 12h/24h
    time->day     = BcdToDec(data[3]);
    time->date    = BcdToDec(data[4]);
    time->month   = BcdToDec(data[5]);
    time->year    = BcdToDec(data[6]);
}

void DS1307_SetAlarm(DS1307 *time){
		uint8_t data[4];
		data[0]=DecToBcd(time->seconds) & 0x7F;
		data[1]=DecToBcd(time->minutes) & 0x7F;
		data[2]=DecToBcd(time->hours) & 0x3F;
		data[3] = 0x80;                                                                                                                                                                                                                                                                                                                      
	
		HAL_I2C_Mem_Write(&hi2c1,  DS1307_I2C_ADDR, DS1307_SECS_A1M1, I2C_MEMADD_SIZE_8BIT, data ,4, 100);
		//xem trang thai dieu khien hen gio
		uint8_t ctrl;
		HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDR, DS1307_CONTROL, I2C_MEMADD_SIZE_8BIT, &ctrl, 1, 100);
		ctrl |= 0x05;
		HAL_I2C_Mem_Write(&hi2c1, DS1307_I2C_ADDR, DS1307_CONTROL, I2C_MEMADD_SIZE_8BIT, &ctrl, 1, 100);
		// xem co flag hen gio
		uint8_t status;
		HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDR, DS1307_STATUS, I2C_MEMADD_SIZE_8BIT, &status, 1, 100);
		status &= 0xFE;
		HAL_I2C_Mem_Write(&hi2c1, DS1307_I2C_ADDR, DS1307_STATUS, I2C_MEMADD_SIZE_8BIT, &status, 1, 100);
}
	
void DS1307_GetAlarm(DS1307 *time){
		uint8_t buffer[3];
		HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDR, DS1307_SECS_A1M1, I2C_MEMADD_SIZE_8BIT, buffer, 3, 100);
		time->seconds = BcdToDec(buffer[0] & 0x7F);
    time->minutes = BcdToDec(buffer[1] & 0x7F);
    time->hours   = BcdToDec(buffer[2] & 0x3F);

}