#ifndef CODE_LOGIC_SSD1306_H
#define CODE_LOGIC_SSD1306_H

#include "DS1307.h"
#include "Keypad.h"
#include "SSD1306.h"
#include "FontSSD1306.h"
#include "MH-R38.h"

void Handle_SpcKey(char , int *temp_val, int max_val);
void Logic_Code_SSD1306(void);
void  Led_Int_Handle(void);

#endif