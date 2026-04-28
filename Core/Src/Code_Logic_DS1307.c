#include "Code_Logic_SSD1306.h"
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
void Logic_Code(void )
{
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