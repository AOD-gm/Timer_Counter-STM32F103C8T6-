#ifndef Menu_UI_H
#define Menu_UI_H
#include "main.h"
#include <stdint.h>

extern uint8_t u8_RxBuff[50];
extern uint8_t _RxIndex;
extern uint8_t Rx_Flag;
extern uint8_t alarm_set;
extern int temp_alarm_h, temp_alarm_m, temp_alarm_s;
void UI_ESP_Process(void);
void UI_System_Process(void);
void UI_Proccess_Keypad(char key);
void UI_Proccess_IR(uint32_t IR_Code);
void UI_Render(void);
#endif