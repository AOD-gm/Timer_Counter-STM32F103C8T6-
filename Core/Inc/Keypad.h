// định nghĩa chân hàng
#define R1_Port GPIOB
#define R2_Port GPIOB 
#define R3_Port GPIOB
#define R4_Port GPIOB 
// định nghĩa chân cột
#define C1_Port GPIOA
#define C2_Port GPIOA
#define C3_Port GPIOB
#define C4_Port GPIOB

// định nghĩa chân cắm
#define R1_Pin GPIO_PIN_12
#define R2_Pin GPIO_PIN_13
#define R3_Pin GPIO_PIN_14
#define R4_Pin GPIO_PIN_15

// định nghĩa chân cắm cột
#define C1_Pin GPIO_PIN_12
#define C2_Pin GPIO_PIN_11
#define C3_Pin GPIO_PIN_8
#define C4_Pin GPIO_PIN_9

char Keypad_Read(void);
