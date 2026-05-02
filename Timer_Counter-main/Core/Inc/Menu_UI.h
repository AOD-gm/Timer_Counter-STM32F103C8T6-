#ifndef Menu_UI_H
#define Menu_UI_H
#include "main.h"
#include <stdint.h>

void UI_Proccess_Keypad(char key);
void UI_Proccess_IR(uint32_t IR_Code);
void UI_Render(void);
#endif