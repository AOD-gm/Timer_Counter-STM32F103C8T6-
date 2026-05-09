#include "Esp_01.h"


uint8_t rx_byte;
uint8_t u8_RxBuff[50];
uint8_t RxIndex = 0;
uint8_t Rx_Flag = 0;

extern UART_HandleTypeDef huart1;

void ESP01_UART_Callback(UART_HandleTypeDef *huart) {
  if(huart->Instance == huart1.Instance) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        if(rx_byte != '\n'  && rx_byte != '\r') { 
            u8_RxBuff[RxIndex] = rx_byte; 
            RxIndex++;
            if(RxIndex >= 50) RxIndex = 0; 
        } 
        else { 
            Rx_Flag = 1; 
        }
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1); 
    }
}