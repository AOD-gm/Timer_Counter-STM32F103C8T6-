#include "Keypad.h"
#include "main.h"
const char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

GPIO_TypeDef* Row_Ports[4]={R1_Port, R2_Port, R3_Port, R4_Port};
uint16_t Row_Pins[4]       = {R1_Pin, R2_Pin, R3_Pin, R4_Pin};

GPIO_TypeDef* Col_Ports[4] = {C1_Port, C2_Port, C3_Port, C4_Port};
uint16_t Col_Pins[4]       = {C1_Pin, C2_Pin, C3_Pin, C4_Pin};

char Keypad_Init(void ){
    for(int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(Row_Ports[i], Row_Pins[i], GPIO_PIN_SET);
    }
    for(int i =0; i<4 ; i++){
        HAL_GPIO_WritePin(Row_Ports[i],Row_Pins[i], GPIO_PIN_RESET);
        for(int j=0; j<4;j++){
        if(HAL_GPIO_ReadPin(Col_Ports[j], Col_Pins[j]) ==GPIO_PIN_RESET){
            HAL_Delay(20);
                if(HAL_GPIO_ReadPin(Col_Ports[j], Col_Pins[j]) == GPIO_PIN_RESET) {
                    char pressed_key =keys[i][j];
                    while(HAL_GPIO_ReadPin(Col_Ports[j], Col_Pins[j]) == GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(Row_Ports[i], Row_Pins[i], GPIO_PIN_SET);
                    return pressed_key;
                }
            }
        }
        HAL_GPIO_WritePin(Row_Ports[i], Row_Pins[i], GPIO_PIN_SET);
    }
    return 0;
}