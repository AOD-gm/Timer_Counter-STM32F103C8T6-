#include "Menu_UI.h"
#include "SSD1306.h"
#include "MH-R38.h"
#include "DS1307.h"
int mode = 0;
int menu = 0;
int locate = 0;
int temp_hour = 0, temp_minute = 0, temp_second = 0;
int alarm_set = 0;
int led_stage=0;

DS1307 time;
char time_str[20];
char date_str[20];
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
void UI_Proccess_Keypad(char key){
        if(key==0){
            return;
        }
        if(menu==0){
            if(key=='6'){
                mode++;
                if(mode>3) 
                    mode=0;
                OLED_Clear();
            }
        else if(key=='4'){
            mode--;
            if(mode<0) 
                mode=3;
            OLED_Clear();
        }
    }
        if (mode == 1) {
            if (key == 'A') { menu = 0; locate = 0; OLED_Clear(); }
            else if (key == 'B') { menu = 1; locate = 0; OLED_Clear(); }
            else if (key == 'C') { menu = 2; locate = 0; OLED_Clear(); }
            else if (key == 'D') { menu = 3; locate = 0; OLED_Clear(); }

        // Lưu báo thức
            if (key == '*' && menu >= 1 && menu <= 3) {
                DS1307 set_alarm;
                set_alarm.hours = temp_hour;
                set_alarm.minutes = temp_minute;
                set_alarm.seconds = temp_second;
                DS1307_SetAlarm(&set_alarm); 
                
                OLED_Clear();
                OLED_Print("DA LUU XONG!", 20, 3);
                HAL_Delay(1000); 
                
                alarm_set = 1;
                menu = 0;   
                locate = 0;
                mode = 0; // Văng ra màn hình chính
                OLED_Clear();
                return;
            }

            // Truyền số đang bấm vào hàm nhập
            if (menu == 1) Handle_SpcKey(key, &temp_hour, 23);
            else if (menu == 2) Handle_SpcKey(key, &temp_minute, 59);
            else if (menu == 3) Handle_SpcKey(key, &temp_second, 59);
    }
	}
void UI_Proccess_IR(uint32_t IR_Code){
    static int count=0;
    if(IR_Code==0){
        return;
    }
    switch(IR_Code){
        case RM_Left: 
            UI_Proccess_Keypad('4');
            break;
        case RM_Right:
            UI_Proccess_Keypad('6');
             break;
        case RM_00:
            UI_Proccess_Keypad('0');
             break;
        case RM_01:
            UI_Proccess_Keypad('1');
            break;
        case RM_02:
            UI_Proccess_Keypad('2');
            break;
        case RM_03:
            UI_Proccess_Keypad('3');
            break;
        case RM_04:
            UI_Proccess_Keypad('4');
            break;
        case RM_05:
            UI_Proccess_Keypad('5');
            break;
        case RM_06:
            UI_Proccess_Keypad('6');
            break;
        case RM_07:
            UI_Proccess_Keypad('7');
            break;
        case RM_08:
            UI_Proccess_Keypad('8');
            break;
        case RM_09:
            UI_Proccess_Keypad('9');
            break;  
        case RM_C:
            if(count==0){
                count=1;
                UI_Proccess_Keypad('B');
            }
            else if(count==1){
                count=2;
                UI_Proccess_Keypad('C');
            }
            else if(count==2){
                count=0;
                UI_Proccess_Keypad('D');
            }
            break;
        case RM_Return:
            UI_Proccess_Keypad('#');
            break;
        case RM_Play:
            UI_Proccess_Keypad('*');
            break;
    }
}
void UI_Render(void){
      switch(mode){
			case 10:// Hien Thi Menu
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
                    OLED_Print(alarm_str, 0, 2);
                    if (alarm_info.hours == time.hours && 
                        alarm_info.minutes == time.minutes && 
                        alarm_info.seconds == time.seconds ) {
                            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0); 
                            
                            // 2. Kích hoạt LUÔN chân PC13 (Để chớp cái đèn xanh trên board nhìn cho sướng mắt)
                            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); 
                            
                            OLED_Clear();
                            
                            // 3. Đọc trạng thái (Đọc PA0 hay PC13 đều được vì nãy giờ mình cho nó lật cùng nhau)
                            if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
                                OLED_Print("Den da SANG!", 15, 3);
                            } else {
                                OLED_Print("Den da TAT!", 15, 3);
                            }
                            
                            HAL_Delay(2000);
                                alarm_set = 0;
                }
							}
				break;
            case 1: // Màn hình Setting
            if (menu == 10) {
                OLED_Print("Setting", 36, 0); 
                OLED_Print("Nhan A xem HD", 10, 2);
            }
            else if (menu == 0) {
                OLED_Print("B:Gio C:Phut D:Giay", 0, 1);
                OLED_Print("*:Luu  #:Xoa", 0, 3);
            }
            else if (menu >= 1 && menu <= 3) {
                if (menu == 1) OLED_Print("Chinh gio!", 32, 6);
                if (menu == 2) OLED_Print("Chinh phut!", 32, 6);
                if (menu == 3) OLED_Print("Chinh giay!", 32, 6);
                
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
