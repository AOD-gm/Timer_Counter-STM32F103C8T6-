#include "HLK_LD2410.h"

static uint8_t rx_data;          
static uint8_t rx_buffer[32];    
static uint8_t rx_index = 0;     
UART_HandleTypeDef *ld2410_huart; 

LD2410_Data_t Radar;
void HLK_LD2410_Init(UART_HandleTypeDef *huart) {
    ld2410_huart = huart;
    Radar.state = 0;
    Radar.moving_dist = 0;
    Radar.moving_energy = 0;
    Radar.static_dist = 0;
    Radar.static_energy = 0;
    HAL_UART_Receive_IT(ld2410_huart, &rx_data, 1);

}

void LD2410_UART_Callback(UART_HandleTypeDef *huart){
    if(huart->Instance == ld2410_huart->Instance){
        rx_buffer[rx_index++] = rx_data;
        if(rx_index==4){
            if (rx_buffer[0] != 0xF4 || rx_buffer[1] != 0xF3 || 
                rx_buffer[2] != 0xF2 || rx_buffer[3] != 0xF1) {
                rx_buffer[0] = rx_buffer[1];
                rx_buffer[1] = rx_buffer[2];
                rx_buffer[2] = rx_buffer[3];
                rx_index = 3;
            }
        }
        if(rx_index >= 8){
            if(rx_buffer[rx_index-4] == 0xF8 && rx_buffer[rx_index-3] == 0xF7 && 
               rx_buffer[rx_index-2] == 0xF6 && rx_buffer[rx_index-1] == 0xF5) {
                
                // Đã bắt được nguyên 1 khung Data xịn!
                Radar.state         = rx_buffer[8];
                Radar.moving_dist   = rx_buffer[9] | (rx_buffer[10] << 8);
                Radar.moving_energy = rx_buffer[11];
                Radar.static_dist   = rx_buffer[12] | (rx_buffer[13] << 8);
                Radar.static_energy = rx_buffer[14];
                
                rx_index = 0; // Đón frame mới
            }
        }
        if(rx_index >= 30){
            rx_index = 0;
        }
        HAL_UART_Receive_IT(ld2410_huart, &rx_data, 1);
    }
}

void HLK_LD2410_Enable(void){
    uint8_t cmd[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFF, 0x00, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
    HAL_UART_Transmit(ld2410_huart, cmd, 14, 1000);
    HAL_Delay(100);

}
void HLK_LD2410_Disable(void){
    uint8_t cmd[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFE, 0x00, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
    HAL_UART_Transmit(ld2410_huart, cmd, 14, 1000);
    HAL_Delay(100);
}

void LD2410_Restart(void) {
    HLK_LD2410_Disable();
    uint8_t cmd[12] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA3, 0x00, 0x04, 0x03, 0x02, 0x01};
    HAL_UART_Transmit(ld2410_huart, cmd, 12, 1000);
    HAL_Delay(500);
}

void LD2410_Set_Threshold(uint8_t gate, uint8_t move_sens, uint8_t static_sens) {
    HLK_LD2410_Enable();

    
    uint8_t cmd[28] = {
        0xFD, 0xFC, 0xFB, 0xFA, // Head
        0x12, 0x00,             // Length: 18 byte dữ liệu
        0x64, 0x00,             // Command: 0x0064
        gate, 0x00, 0x00, 0x00, // Vùng muốn chỉnh (4 byte)
        move_sens, 0x00, 0x00, 0x00,   // Độ nhạy động (4 byte)
        static_sens, 0x00, 0x00, 0x00, // Độ nhạy tĩnh (4 byte)
        0x04, 0x03, 0x02, 0x01  // End
    };

    HAL_UART_Transmit(ld2410_huart, cmd, 24, 1000);
    HAL_Delay(50);
    
    HLK_LD2410_Disable();
}

void LD2410_Save_Config(void) {
    HLK_LD2410_Enable();
    uint8_t cmd[12] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x61, 0x00, 0x04, 0x03, 0x02, 0x01};
    HAL_UART_Transmit(ld2410_huart, cmd, 12, 1000);
    HAL_Delay(50);
    HLK_LD2410_Disable();
}