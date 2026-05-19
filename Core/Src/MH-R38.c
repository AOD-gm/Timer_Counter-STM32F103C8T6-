#include "MH-R38.h"

 TIM_HandleTypeDef *ir_htim;
uint32_t IR_Time_Last = 0; 
uint16_t IR_Diff = 0;
uint32_t IR_Value = 0;
uint8_t  IR_Bit_Count = 0;
uint32_t RM_Code = 0;
uint8_t  New_Ir_Flag = 0;
void IR_Init(TIM_HandleTypeDef *htim){
    ir_htim = htim;
    HAL_TIM_IC_Start_IT(ir_htim, TIM_CHANNEL_2);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
        uint32_t IR_Time_Current = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
        if (IR_Time_Current > IR_Time_Last) {
            IR_Diff = IR_Time_Current - IR_Time_Last;
        } else {
            IR_Diff = (0xFFFF - IR_Time_Last) + IR_Time_Current;
        }
        IR_Time_Last = IR_Time_Current;
        if (IR_Diff > 13000 && IR_Diff < 14000) {
            IR_Bit_Count = 0;
            IR_Value = 0;
        } 
        else if (IR_Diff > 1000 && IR_Diff < 2500) {
            IR_Value <<= 1;
            if (IR_Diff > 1800) {
                IR_Value |= 1;
            }
            IR_Bit_Count++;
        }
        if (IR_Bit_Count >= 32) {
            RM_Code = IR_Value;
            New_Ir_Flag = 1;
            IR_Bit_Count = 0;
        }
    }
}

uint32_t Read_RM(void){
    if (New_Ir_Flag){
        New_Ir_Flag = 0;
        return RM_Code;
    }
    return 0;
}
