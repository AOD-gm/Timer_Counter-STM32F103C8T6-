/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DS1307.h"
#include "Keypad.h"
#include "SSD1306.h"
#include "FontSSD1306.h"
/* USER CODE END Includes */

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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
		int mode=10;
		int menu=10;

	DS1307 time = {.seconds = 00, .minutes = 49, .hours = 14, .day = 7, .date = 25, .month = 4, .year = 26};
		int temp_hour;
		int temp_minute;
		int temp_second;
		int locate=0;
		char pressed_key;
		int alarm_set=0;
	void Handle_SpcKey(char key, int *temp_val, int max_val){
				if (key >= '0' && key <= '9') {
					uint8_t pressed_key = key - '0'; 
						if (locate == 0) {
							int max_chuc = max_val / 10;
							if(pressed_key<=max_chuc){
									*temp_val = pressed_key * 10;
									locate = 1;
							}
							} 
						else if (locate == 1) {
							int tong_tam = *temp_val + pressed_key;
							if(tong_tam<=max_val){
									*temp_val = tong_tam;
									locate = 2;
							}
							}
			}
				else if (key=='#'){
						if(locate==2){
							locate=1;
							*temp_val = (temp_hour/10)*10;
						}
						else if(locate ==1){
							locate=0;
							*temp_val=0;
						}
			}
	}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	char time_str[20];
	char date_str[20];
		char buffer[20];


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
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
	DS1307_Init();
	OLED_Init();
	OLED_Clear();
uint8_t stop_1hz = 0x04;
  HAL_I2C_Mem_Write(&hi2c1, (0x68 << 1), 0x0E, I2C_MEMADD_SIZE_8BIT, &stop_1hz, 1, 100);
  uint8_t clear_flag;
  HAL_I2C_Mem_Read(&hi2c1, (0x68 << 1), 0x0F, I2C_MEMADD_SIZE_8BIT, &clear_flag, 1, 100);
  clear_flag &= ~0x01; // Xóa bit A1F
  HAL_I2C_Mem_Write(&hi2c1, (0x68 << 1), 0x0F, I2C_MEMADD_SIZE_8BIT, &clear_flag, 1, 100);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//  DS1307_SetTime(&time);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		int change=1;
		char key=Keypad_Init();
		if (mode==1 && change==1){
			change=0;
		}
		if (key == '8' && change==1) {
            mode++;
            if (mode > 3) mode = 0; 
						
            OLED_Clear(); 
        }
		else if(key=='2' && change==1 ){
						mode--;
						if(mode<0) 
						mode =3;
						OLED_Clear();
		}
		switch(mode){
			OLED_Clear();
			case 10:
				OLED_Print("Main Menu", 36,1);
				OLED_Print("Setting", 40,3);
				OLED_Print("Set Alarm", 36,5);
				break;
			case 0: // Hien Thi Time (man hinh chinh)
				DS1307_GetTime(&time);
				OLED_Print("Main Menu", 36, 0);
				sprintf(time_str, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
				OLED_Print(time_str,0,6);
				sprintf(date_str, "T%d %02d/%02d/20%02d", time.day, time.date, time.month, time.year);
				OLED_Print(date_str,0,7);
				if (alarm_set == 1) {
										DS1307 alarm_info;
										DS1307_GetAlarm(&alarm_info);
                    char alarm_str[20];
                    sprintf(alarm_str, "ALARM: %02d:%02d:%02d", alarm_info.hours, alarm_info.minutes, alarm_info.seconds);
										if (alarm_info.hours == time.hours && 
                        alarm_info.minutes == time.minutes && 
                        alarm_info.seconds == time.seconds){
											OLED_Print("Den da dc tat", 0,3);
											
										}
                    OLED_Print(alarm_str, 0, 2);
                }
				break;
			case 1:

				if(key=='A'){
					menu=0;
					locate=0;
					OLED_Clear();

				}
				else if(key=='B'){
					menu=1;
					locate=0;
					OLED_Clear();

				}
				else if(key=='C'){
					menu=2;
					locate=0;
					OLED_Clear();
				}
				else if(key=='D'){
					menu=3;
					locate=0;
					OLED_Clear();

				}

				if (key == '*' && menu >= 1 && menu <= 3) {
						DS1307 set_alarm;
						set_alarm.hours = temp_hour;
						set_alarm.minutes = temp_minute;
						set_alarm.seconds = temp_second;
						DS1307_SetAlarm(&set_alarm); 
						
						OLED_Clear();
						OLED_Print("DA LUU XONG!", 20, 3);
						HAL_Delay(1000);
						alarm_set=1;
						locate = 0;
						mode = 0; // Vang ra m�n h�nh ch�nh
						OLED_Clear();

						break;
				}
				if (menu == 10) {
						OLED_Print("Setting", 36, 0); 
						OLED_Print("Nhan A xem HD", 10, 2);
				}
				else if (menu == 0) {
						OLED_Print("B:Gio C:Phut D:Giay", 0, 1);
						OLED_Print("*:Luu  #:Xoa", 0, 3);
				}
				else if (menu >= 1 && menu <= 3) {
						if (menu == 1) {
							OLED_Print("Chinh gio!", 32, 6);
							Handle_SpcKey(key, &temp_hour,23);
					}
						else if (menu == 2) {
							OLED_Print("Chinh phut!", 32, 6);
							Handle_SpcKey(key, &temp_minute,59);
						}
						else if (menu == 3) {
							OLED_Print("Chinh giay!", 32, 6);
							Handle_SpcKey(key, &temp_second,59);
						}
						
							OLED_Print("CAI DAT THOI GIAN", 10, 0); 

							char buf_h[20], buf_m[20], buf_s[20];

							if (menu == 1) sprintf(buf_h, "* GIO : %02d", temp_hour);     
							else           sprintf(buf_h, "  GIO : %02d", temp_hour);

							if (menu == 2) sprintf(buf_m, "* PHUT: %02d", temp_minute); 
							else           sprintf(buf_m, "  PHUT: %02d", temp_minute);

							if (menu == 3) sprintf(buf_s, "* GIAY: %02d", temp_second); 
							else           sprintf(buf_s, "  GIAY: %02d", temp_second);

							OLED_Print(buf_h, 0, 2); 
							OLED_Print(buf_m, 0, 3); 
							OLED_Print(buf_s, 0, 4); 
							
							OLED_Print("*:Luu      #:Xoa", 0, 7);
				}
			break;

			}

  }
		HAL_Delay(50);
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    if(GPIO_Pin == GPIO_PIN_0) 
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        
        uint8_t status;
        HAL_I2C_Mem_Read(&hi2c1, (0x68 << 1), 0x0F, I2C_MEMADD_SIZE_8BIT, &status, 1, 100);
        status &= ~0x01; // Xóa bit A1F (Bit số 0)
        HAL_I2C_Mem_Write(&hi2c1, (0x68 << 1), 0x0F, I2C_MEMADD_SIZE_8BIT, &status, 1, 100);
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
  /* User can add his own implementation to report the HAL error return state */
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
