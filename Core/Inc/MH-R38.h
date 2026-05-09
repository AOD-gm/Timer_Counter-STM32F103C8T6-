#include "main.h"

#define RM_00 0x00FF6897
#define RM_01 0x00FF30CF
#define RM_02 0x00FF18E7
#define RM_03 0x00FF7A85
#define RM_04 0x00FF10EF
#define RM_05 0x00FF38C7
#define RM_06 0x00FF5AA5
#define RM_07 0x00FF42BD
#define RM_08 0x00FF4AB5
#define RM_09 0x00FF52AD

#define RM_ON_OFF 0x00FFA25D
#define RM_Menu 0x00FFE21D
#define RM_Plus 0x00FF02FD
#define RM_Minus 0x00FF9867
#define RM_Left 0x00FFE01F
#define RM_Right 0x00FF906F

#define RM_Test 0x00FF22DD
#define RM_Return 0x00FFC23D
#define RM_C  0x00FFB04F
#define RM_Play 0x00FFA857

extern uint8_t  New_Ir_Flag; 
extern uint32_t RM_Code;

void IR_Init(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
uint32_t Read_RM(void);

