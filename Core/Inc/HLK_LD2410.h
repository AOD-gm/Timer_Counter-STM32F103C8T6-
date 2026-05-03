#ifndef __HLK_LD2410_H
#define __HLK_LD2410_H

#include "SSD1306.h"

typedef struct {
    uint8_t  state;           // 0: Trống, 1: Động, 2: Tĩnh, 3: Động+Tĩnh
    uint16_t moving_dist;     // Khoảng cách chuyển động (cm)
    uint8_t  moving_energy;   // Năng lượng chuyển động (%)
    uint16_t static_dist;     // Khoảng cách đứng yên (cm)
    uint8_t  static_energy;   // Năng lượng đứng yên (%)
} LD2410_Data_t;
void HLK_LD2410_Init(UART_HandleTypeDef *huart);
void LD2410_UART_Callback(UART_HandleTypeDef *huart);

void HLK_LD2410_Enable(void);
void HLK_LD2410_Disable(void);
void LD2410_Save_Config(void);
void LD2410_Restart(void);
void LD2410_Set_Threshold(uint8_t gate, uint8_t move_sens, uint8_t static_sens);

extern LD2410_Data_t Radar;
#endif /* __HLK_LD2410_H */