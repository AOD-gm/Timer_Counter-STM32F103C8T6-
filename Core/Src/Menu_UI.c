	#include "Menu_UI.h"
	#include "SSD1306.h"
	#include "MH-R38.h"
	#include "DS1307.h"
	#include <stdio.h> // Nhớ include cái này để dùng sprintf

	int mode = 0;
	int menu = 0;
	int locate = 0;
	int temp_hour = 0, temp_minute = 0, temp_second = 0, temp_date, temp_month, temp_year,temp_day;
	int alarm_set = 0;
	int led_stage = 0;
	uint8_t setting = 0;
	uint8_t choose = 0;
	DS1307 time;
	char time_str[20];
	char date_str[20];

	// 1. HÀM XỬ LÝ NHẬP SỐ
	void Handle_SpcKey(char key, int *temp_val, int max_val) {
			if (key >= '0' && key <= '9') {
					uint8_t pressed_key = key - '0'; 
					if (locate == 0) {
							int max_chuc = max_val / 10;
							if (pressed_key <= max_chuc) {
									*temp_val = pressed_key * 10;
									locate = 1;
							}
					} 
					else if (locate == 1) {
							int tong_tam = *temp_val + pressed_key;
							if (tong_tam <= max_val) {
									*temp_val = tong_tam;
									locate = 2;
							}
					}
			} 
			else if (key == '#') {
					if (locate == 2) {
							locate = 1;
							*temp_val = (*temp_val / 10) * 10;
					} else if (locate == 1) {
							locate = 0;
							*temp_val = 0;
					}
			}
	}

	// 2. HÀM LƯU VÀ THOÁT
	void Key_Save_Return(char key) {
			if (key == '*') {
					// --- CHỌN MỤC TRONG MENU ---
					if (menu == 1) {
							if (mode == 0) menu = 3;      // Vào Mode
							else if (mode == 1) { menu = 2; setting = 0; } // Vào Setting
							else if (mode == 2) menu = 4; // Vào Radar
							OLED_Clear();
							OLED_Print("DA CHON!", 35, 3);
							HAL_Delay(500);
							OLED_Clear();
					}
					// --- LƯU THỜI GIAN/BÁO THỨC TRONG SETTING ---
					else if (menu == 2) {
							if (choose == 1) {
								if(setting >= 1 && setting <= 3){
									// LƯU TIMER
									DS1307 set_timer;
									set_timer.day = temp_day;
									set_timer.date = temp_date;
									set_timer.month = temp_month;
									set_timer.year = temp_year;
									set_timer.hours = temp_hour;
									set_timer.minutes = temp_minute;
									set_timer.seconds = temp_second;
									DS1307_SetTime(&set_timer); 
									OLED_Clear();
									OLED_Print("Timer Updated!", 15, 3);
								}
								else if(setting>=5 && setting <=8){
									setting =1;
									locate=0;
									OLED_Clear();
								}
							} 
							else if (choose == 2) {
									// LƯU ALARM
									DS1307 set_alarm;
									set_alarm.hours = temp_hour;
									set_alarm.minutes = temp_minute;
									set_alarm.seconds = temp_second;
									DS1307_SetAlarm(&set_alarm); 
									alarm_set = 1;
									OLED_Clear();
									OLED_Print("Alarm Saved!", 20, 3);
							}
							HAL_Delay(1000);
							menu = 0; setting = 0; locate = 0; mode = 0; // Trở về màn hình chờ
							OLED_Clear();
					}
			} 
			else if (key == '#') {
					// --- THOÁT RA ---
					if (menu == 1) { // Thoát khỏi Menu chính
							menu = 0; 
							OLED_Clear(); 
							HAL_Delay(300);
					} 
					else if (menu == 2) { 
							// CHỈ THOÁT RA KHI ĐANG Ở MÀN HÌNH CHỌN (setting 0 hoặc 4)
							if (setting == 0 || setting == 4) { 
									menu = 1; 
									setting = 0; 
									OLED_Clear(); 
									HAL_Delay(300); 
							}
					}
					else if (menu == 3 || menu == 4) { // Thoát khỏi Radar hoặc Mode
							menu = 1; 
							OLED_Clear(); 
							HAL_Delay(300);
					}
			}
	}

	// 3. HÀM ĐIỀU HƯỚNG PHÍM BẤM TỔNG TÀI
	void UI_Proccess_Keypad(char key) {
			if (key == 0) return;

			// --- MÀN HÌNH CHỜ ---
			if (menu == 0) {
					if (key == 'A') { mode = 0; menu = 1; OLED_Clear(); } // Vào Menu
					else if (key == 'B') { menu = 2; setting = 0; OLED_Clear(); } // Shortcut vào Setting
					return;
			}

			// --- MÀN HÌNH MENU CHÍNH ---
			if (menu == 1) {
					if (key == '2') { mode--; if (mode < 0) mode = 2; OLED_Clear(); }
					else if (key == '8') { mode++; if (mode > 2) mode = 0; OLED_Clear(); }
					Key_Save_Return(key); // Xử lý nhấn '*' để chọn, '#' để thoát
					return;
			}

			// --- MÀN HÌNH SETTING ---
			if (menu == 2) {
					if (setting == 0) { // Đang ở Menu chọn loại Setting
							if (key == '4') { choose = 1; setting = 4; OLED_Clear(); }
							else if (key == '6') { choose = 2; setting = 4; OLED_Clear(); }
					} 
					else if (setting == 4) { // Đang ở bảng Hướng dẫn, chờ chọn B C D
							
								if (key == 'A') { setting = 5; locate = 0; OLED_Clear(); } 
							else if (key == 'B') { setting = 6; locate = 0; OLED_Clear(); } 
							else if (key == 'C') { setting = 7; locate = 0; OLED_Clear(); } 
							else if (key == 'D') { setting = 8; locate = 0; OLED_Clear(); }
					} 
					else if (setting >= 5 && setting <= 8) { 
							// ĐANG NHẬP NGÀY THÁNG NĂM
								if (key == 'A') { setting = 5; locate = 0; OLED_Clear(); } 
							else if (key == 'B') { setting = 6; locate = 0; OLED_Clear(); } 
							else if (key == 'C') { setting = 7; locate = 0; OLED_Clear(); } 
							else if (key == 'D') { setting = 8; locate = 0; OLED_Clear(); }
							
							else if ((key >= '0' && key <= '9') || key == '#') {
                if (setting == 6)      Handle_SpcKey(key, &temp_date, 31);
                else if (setting == 7) Handle_SpcKey(key, &temp_month, 12);
                else if (setting == 8) Handle_SpcKey(key, &temp_year, 99);
                else if (setting == 5) Handle_SpcKey(key, &temp_day, 7);
            }
					}
					else if (setting >= 1 && setting <= 3) { // Đang nhập số
							if (key == 'B') { setting = 1; locate = 0; OLED_Clear(); }
							else if (key == 'C') { setting = 2; locate = 0; OLED_Clear(); }
							else if (key == 'D') { setting = 3; locate = 0; OLED_Clear(); }
							else if ((key >= '0' && key <= '9') || key == '#') {
								if (setting == 1) Handle_SpcKey(key, &temp_hour, 23);
								else if (setting == 2) Handle_SpcKey(key, &temp_minute, 59);
								else if (setting == 3) Handle_SpcKey(key, &temp_second, 59);
					}
							}
					
					Key_Save_Return(key);
					return;
			}
			
			// --- MÀN HÌNH RADAR & MODE ---
			if (menu == 3 || menu == 4) {
					Key_Save_Return(key);
					return;
			}
	}

	// 4. HÀM XỬ LÝ HỒNG NGOẠI
	void UI_Proccess_IR(uint32_t IR_Code) {
			static int count = 0;
			if (IR_Code == 0) return;
			
			switch (IR_Code) {
					case RM_Left: UI_Proccess_Keypad('4'); break;
					case RM_Right: UI_Proccess_Keypad('6'); break;
					case RM_Plus:  UI_Proccess_Keypad('2'); break; 
					case RM_Minus: UI_Proccess_Keypad('8'); break;
					case RM_00: UI_Proccess_Keypad('0'); break;
					case RM_01: UI_Proccess_Keypad('1'); break;
					case RM_02: UI_Proccess_Keypad('2'); break;
					case RM_03: UI_Proccess_Keypad('3'); break;
					case RM_04: UI_Proccess_Keypad('4'); break;
					case RM_05: UI_Proccess_Keypad('5'); break;
					case RM_06: UI_Proccess_Keypad('6'); break;
					case RM_07: UI_Proccess_Keypad('7'); break;
					case RM_08: UI_Proccess_Keypad('8'); break;
					case RM_09: UI_Proccess_Keypad('9'); break;  

					case RM_Return: UI_Proccess_Keypad('#'); break;
					case RM_Play: UI_Proccess_Keypad('*'); break;
					case RM_Test:   UI_Proccess_Keypad('A'); break; 
					case RM_Menu:   UI_Proccess_Keypad('B'); break;
					case RM_C:      UI_Proccess_Keypad('C'); break; 
					case RM_ON_OFF: UI_Proccess_Keypad('D'); break;
			}
	}

	// 5. HÀM HIỂN THỊ
	void UI_Render(void) {
			switch (menu) {
					case 0: // MÀN HÌNH CHỜ
							DS1307_GetTime(&time);
							OLED_Print("WAIT MENU", 36, 0);
							sprintf(time_str, "%02d:%02d", time.hours, time.minutes);
							OLED_Print(time_str, 45, 1);
							sprintf(date_str, "T%d %02d/%02d/20%02d", time.day, time.date, time.month, time.year);
							OLED_Print(date_str, 20, 2);
							
							if (alarm_set == 1) {
									DS1307 alarm_info;
									DS1307_GetAlarm(&alarm_info);
									char alarm_str[20];
									sprintf(alarm_str, "ALARM: %02d:%02d:%02d", alarm_info.hours, alarm_info.minutes, alarm_info.seconds);
									OLED_Print(alarm_str, 0, 4);
									
									// Logic Kích hoạt báo thức
									if (alarm_info.hours == time.hours && 
											alarm_info.minutes == time.minutes && 
											alarm_info.seconds == time.seconds) {
											HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0); 
											HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); 
											OLED_Clear();
											
											if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
													OLED_Print("Den da SANG!", 15, 3);
											} else {
													OLED_Print("Den da TAT!", 15, 3);
											}
											HAL_Delay(1000);
											alarm_set = 0;
									}
							}
							break;
							
					case 1: // MÀN HÌNH MENU CHÍNH
							OLED_Print("MENU", 45, 0);
							OLED_Print("Mode", 45, 2);
							OLED_Print("Setting", 45, 4);
							OLED_Print("Radar", 45, 6);
							
							// Vẽ hộp chọn
							if (mode == 0) OLED_DrawSelectionBox(40, 2, 35);
							else if (mode == 1) OLED_DrawSelectionBox(40, 4, 50);
							else if (mode == 2) OLED_DrawSelectionBox(40, 6, 45);
							
							OLED_Print("#:Return", 0, 7);
							OLED_Print("*:Save", 80, 7);
							break;
							
					case 2: // MÀN HÌNH SETTING
							if (setting == 0) {
									OLED_Print("--- THIET LAP ---", 15, 0); 
									OLED_Print("4: Chinh Timer", 10, 2);
									OLED_Print("6: Chinh Alarm", 10, 4);
									OLED_Print("#: Quay lai", 0, 7);
							}
							else if (setting == 4) {
									OLED_Print("--- CHON MUC ---", 15, 0); 
									OLED_Print("B:Gio C:Phut D:Giay", 0, 3);
									OLED_Print("#: Quay lai", 0, 7);
							}
							else if (setting >= 5 && setting <= 8) { 
									// GIAI ĐOẠN 1: VẼ MÀN HÌNH CÀI NGÀY
									OLED_Print("BUOC 1: NGAY THANG", 10, 0); 

									char buf_d[20], buf_m[20], buf_y[20],buf_day[20];
									sprintf(buf_day, "%s T : %02d", (setting == 8 ? "*" : " "), temp_day);
									sprintf(buf_d, "%s NGAY : %02d", (setting == 5 ? "*" : " "), temp_date);     
									sprintf(buf_m, "%s THANG: %02d", (setting == 6 ? "*" : " "), temp_month);
									sprintf(buf_y, "%s NAM  : 20%02d", (setting == 7 ? "*" : " "), temp_year);
									
									OLED_Print(buf_day, 15, 2);
									OLED_Print(buf_d, 15, 3); 
									OLED_Print(buf_m, 15, 4); 
									OLED_Print(buf_y, 15, 5); 
									
									OLED_Print("*:Next      #:Xoa", 0, 7); // Phím * bây giờ là Next
							}
							else if (setting >= 1 && setting <= 3) {
									if (setting == 1) OLED_Print("Dang chinh Gio!", 15, 6);
									else if (setting == 2) OLED_Print("Dang chinh Phut!", 15, 6);
									else if (setting == 3) OLED_Print("Dang chinh Giay!", 15, 6);
									
									OLED_Print("CAI DAT THOI GIAN", 10, 0); 

									char buf_h[20], buf_m[20], buf_s[20];
									if (setting == 1) sprintf(buf_h, "* GIO  : %02d", temp_hour);     
									else              sprintf(buf_h, "  GIO  : %02d", temp_hour);

									if (setting == 2) sprintf(buf_m, "* PHUT : %02d", temp_minute); 
									else              sprintf(buf_m, "  PHUT : %02d", temp_minute);

									if (setting == 3) sprintf(buf_s, "* GIAY : %02d", temp_second); 
									else              sprintf(buf_s, "  GIAY : %02d", temp_second);

									OLED_Print(buf_h, 10, 2); 
									OLED_Print(buf_m, 10, 3); 
									OLED_Print(buf_s, 10, 4); 
									
									OLED_Print("*:Luu      #:Xoa", 0, 7);
							}
							break;
							
					case 3: // MÀN HÌNH CHẾ ĐỘ MODE
							OLED_Print("--- CHE DO MODE ---", 10, 0);
							OLED_Print("LED Status:", 10, 2);
							if (led_stage == 1) OLED_Print("ON ", 90, 2);
							else                OLED_Print("OFF", 90, 2);
							OLED_Print("#: Thoat", 0, 7);
							break;
							
					case 4: // MÀN HÌNH RADAR
							OLED_Print("--- RADAR SCAN ---", 10, 0);
							OLED_Print("Distance: Wait...", 10, 3);
							OLED_Print("#: Thoat", 0, 7);
							break;
			}
	}