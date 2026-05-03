#include "main.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define Oled_Adr 0x3C << 1
#define Oled_Control 0x00
#define Oled_Data 0x40

extern I2C_HandleTypeDef hi2c1; 
void OLED_Init(void);
void OLED_Clear();
void Oled_SendCmd(uint8_t);
void Oled_SendData(uint8_t);
void OLED_Yellow(void);
void OLED_Blue(void);
void OLED_Cusor(uint8_t x, uint8_t y);
void OLED_Write(char c, uint8_t x, uint8_t y);
void OLED_Print(char* str, uint8_t x, uint8_t y);
void OLED_DrawPixel(uint8_t x, uint8_t y);
void OLED_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r);
void OLED_DrawSelectionBox(uint8_t x0, uint8_t y0, uint8_t width);
void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void OLED_DrawHalfCircle(uint16_t x0, uint16_t y0, uint16_t r);
void OLED_DrawSelectionBracket(uint8_t x0, uint8_t y0, uint8_t width);