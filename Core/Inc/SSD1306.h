#include "main.h"
#include <stdio.h>

#define Oled_Adr 0x3C << 1
#define Oled_Control 0x00
#define Oled_Data 0x40

extern I2C_HandleTypeDef hi2c2; 
void OLED_Init(void);
void OLED_Clear();
void Oled_SendCmd(uint8_t);
void Oled_SendData(uint8_t);
void OLED_Yellow(void);
void OLED_Blue(void);
void OLED_Cusor(uint8_t x, uint8_t y);
void OLED_Write(char c, uint8_t x, uint8_t y);
void OLED_Print(char* str, uint8_t x, uint8_t y);