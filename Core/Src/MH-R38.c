#include "MH-R38.h"

 TIM_HandleTypeDef *ir_htim;
 uint16_t IR_Time1 = 0, IR_Time2 = 0, IR_Diff = 0;
 uint32_t IR_Value = 0;
 uint8_t  IR_Bit_Count = 0;
 uint32_t RM_Code = 0;
 uint8_t  New_Ir_Flag = 0;
void IR_Init(TIM_HandleTypeDef *htim){
    ir_htim = htim;
    HAL_TIM_Base_Start(ir_htim);
}

void IR_EXTI_Handler(void){
    IR_Time1 = __HAL_TIM_GET_COUNTER(ir_htim);
    IR_Diff = IR_Time1 - IR_Time2;
    IR_Time2 = IR_Time1;
    if(IR_Diff >10000 && IR_Diff <15000){
        IR_Bit_Count=0;
        IR_Value=0;
        
    }else if(IR_Diff >500 && IR_Diff <3000){
        IR_Value <<=1;
        if(IR_Diff > 1600){
            IR_Value |=1;
        }
        IR_Bit_Count++;
    }
    if (IR_Bit_Count >= 32){
        RM_Code = IR_Value;
        New_Ir_Flag = 1;
        IR_Bit_Count = 0;
    }
    
}

