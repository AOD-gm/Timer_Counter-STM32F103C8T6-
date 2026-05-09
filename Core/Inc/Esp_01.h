#ifndef ESP_01_H
#define ESP_01_H
#include "main.h"

extern uint8_t rx_byte;
extern uint8_t u8_RxBuff[50];
extern uint8_t RxIndex;
extern uint8_t Rx_Flag;
void ESP01_UART_Callback(UART_HandleTypeDef *huart);

#endif