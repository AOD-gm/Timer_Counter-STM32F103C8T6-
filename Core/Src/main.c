/* USER CODE BEGIN Header */
/**
  ****************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ****************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ****************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
uint8_t u8_RxBuff[20];
uint8_t u8_RxData;
uint8_t u8_TxBuff[20];
uint8_t _RxIndex;
uint16_t Tx_Flag;
/* USER CODE END Includes */
char current_hour[3], current_min[3], current_sec[3];
char alarm_hour[3], alarm_min[3], alarm_sec[3];
char device_state;
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Hàm chuyển đổi số nguyên sang định dạng BCD cho thanh ghi DS1307
uint8_t Dec2Bcd(uint8_t val) {
    return (uint8_t)( (val / 10 * 16) + (val % 10) );
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart1, u8_TxBuff, sizeof(u8_TxBuff), 100);
	HAL_UART_Receive_IT(&huart1, &u8_RxData,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

/* USER CODE BEGIN 3 */
    if(Tx_Flag && _RxIndex >= 10)
    {
        // ---------------- 1. THIẾT LẬP THỜI GIAN THỰC (DS1307) ----------------
        if(u8_RxBuff[0] == 'T')
        {
            strncpy(current_hour, (char*)&u8_RxBuff[2], 2);
            current_hour[2] = '\0';

            strncpy(current_min, (char*)&u8_RxBuff[5], 2);
            current_min[2] = '\0';

            strncpy(current_sec, (char*)&u8_RxBuff[8], 2);
            current_sec[2] = '\0';

            // Chuyển chuỗi thành số nguyên
            uint8_t h = atoi(current_hour);
            uint8_t m = atoi(current_min);
            uint8_t s = atoi(current_sec);

            // Chuyển sang chuẩn BCD và ghi vào thanh ghi DS1307
            uint8_t rtc_time[3];
            // Bit 7 của thanh ghi 0x00 (CH) phải bằng 0 để bộ dao động chạy
            rtc_time[0] = Dec2Bcd(s) & 0x7F; // Thanh ghi 0x00: Giây
            rtc_time[1] = Dec2Bcd(m);        // Thanh ghi 0x01: Phút
            rtc_time[2] = Dec2Bcd(h);        // Thanh ghi 0x02: Giờ

            // Ghi 3 byte vào DS1307 bắt đầu từ địa chỉ 0x00
            HAL_I2C_Mem_Write(&hi2c1, 0xD0, 0x00, I2C_MEMADD_SIZE_8BIT, rtc_time, 3, 100);
        }
		
        // ---------------- 2. THIẾT LẬP BÁO THỨC (Xử lý bằng phần mềm) ----------------
        else if(u8_RxBuff[0] == 'A') 
        {
            // DS1307 không có thanh ghi báo thức phần cứng. 
            // STM32 lưu dữ liệu vào biến để tự so sánh trong vòng lặp.
            strncpy(alarm_hour, (char*)&u8_RxBuff[2], 2);
            alarm_hour[2] = '\0';

            strncpy(alarm_min, (char*)&u8_RxBuff[5], 2);
            alarm_min[2] = '\0';

            strncpy(alarm_sec, (char*)&u8_RxBuff[8], 2);
            alarm_sec[2] = '\0';
        }
     
        // ---------------- 3. ĐIỀU KHIỂN THIẾT BỊ ----------------
        else if(u8_RxBuff[0] == 'D') 
        {
            device_state = u8_RxBuff[2]; // '1' hoặc '0'

            if(device_state == '1') {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); 
            } else {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);  
            }
        }

        memset(u8_RxBuff, 0, sizeof(u8_RxBuff));
        _RxIndex = 0;
        Tx_Flag = 0;  
    }
    /* USER CODE END 3 */
}
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        // N?u chua ph?i ký t? k?t thúc
        if(u8_RxData != 13 && u8_RxData != '\n')
        {
            if(_RxIndex < sizeof(u8_RxBuff) - 1)
            {
                u8_RxBuff[_RxIndex++] = u8_RxData;
            }
        }
        else // g?p ký t? k?t thúc (CR ho?c LF)
        {
            u8_RxBuff[_RxIndex] = '\0'; // k?t thúc chu?i
            Tx_Flag = 1;                // báo main x? lý

            // ? KHÔNG reset _RxIndex ? dây
        }

        HAL_UART_Receive_IT(&huart1, &u8_RxData, 1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */