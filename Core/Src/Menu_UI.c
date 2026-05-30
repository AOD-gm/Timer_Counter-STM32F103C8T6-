	#include "Menu_UI.h"
	#define ESP_EN_PIN   GPIO_PIN_10
	#define ESP_EN_PORT  GPIOB
	#define LED_PIN       GPIO_PIN_0
	#define LED_PORT      GPIOA
	uint8_t mode = 0;
	uint8_t menu = 0;
	uint8_t locate = 0;
	uint8_t temp_hour = 0, temp_minute = 0, temp_second = 0, temp_date, temp_month, temp_year,temp_day;
	uint8_t alarm_set = 0;
	uint8_t led_stage = 0;
	uint8_t setting = 0;
	uint8_t choose = 0;
	DS3231 time;
	char time_str[20];
	char date_str[20];
	uint8_t is_bg_drawn = 0;
	uint8_t set_mode=1;
	uint32_t timer_radar = 0;
	uint8_t light_active = 0;
	uint8_t alarm_active = 0; // flag when alarm is currently active (keep light on)
	uint32_t alarm_timer_ms = 0; // timestamp when alarm was triggered
	uint8_t manual_active = 0; // flag when light was turned on manually via web (keep until manual off)
	uint8_t manual_override_off = 0; // when set, ignore auto-on (radar/alarm) for a period
	uint32_t manual_off_until = 0; // timestamp until which auto-on is blocked
	// 1. HÀM XỬ LÝ NHẬP SỐ
	void Handle_SpcKey(char key, uint8_t *temp_val, uint8_t max_val) {
			if (key >= '0' && key <= '9') {
					uint8_t pressed_key = key - '0'; 
					if (locate == 0) {
							uint8_t max_chuc = max_val / 10;
							if (pressed_key <= max_chuc) {
									*temp_val = pressed_key * 10;
									locate = 1;
							}
					} 
					else if (locate == 1) {
							uint8_t tong_tam = *temp_val + pressed_key;
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
									DS3231 set_timer;
									DS3231_GetTime(&set_timer);
									set_timer.hours = temp_hour;
									set_timer.minutes = temp_minute;
									set_timer.seconds = temp_second;
									DS3231_SetTime(&set_timer); 
									DS3231_GetTime(&set_timer);
									{
										char buf[30];
										sprintf(buf, "Now:%02d:%02d:%02d Y:20%02d", set_timer.hours, set_timer.minutes, set_timer.seconds, set_timer.year);
										OLED_Clear();
										OLED_Print("Timer Updated!", 15, 2);
										OLED_Print(buf, 5, 4);
										HAL_Delay(800);
									}
								}
								else if(setting>=5 && setting <=8){
									DS3231 set_date;
									DS3231_GetTime(&set_date);
									set_date.day = temp_day;
									set_date.date = temp_date;
									set_date.month = temp_month;
									set_date.year = 26;
									DS3231_SetTime(&set_date);
									{
										char bufd[30];
										sprintf(bufd, "D:%02d/%02d/20%02d", set_date.date, set_date.month, set_date.year);
										OLED_Clear();
										OLED_Print("Ngay Updated!", 15, 2);
										OLED_Print(bufd, 5, 4);
										HAL_Delay(800);
									}
								}
							HAL_Delay(1000);
							menu = 2; setting = 4; locate = 0; mode = 0; // Trở về setting
							OLED_Clear();
							}
							else if (choose == 2) {
									// LƯU ALARM
									DS3231 set_alarm;
									set_alarm.hours = temp_hour;
									set_alarm.minutes = temp_minute;
									set_alarm.seconds = temp_second;
									DS3231_SetAlarm(&set_alarm); 

									OLED_Clear();
									OLED_Print("Alarm Saved!", 20, 3);
									alarm_set = 1;;
									HAL_Delay(1000);
							menu = 2; setting = 0; locate = 0; mode = 0; // Trở về setting
							OLED_Clear();
							}

					}
					else if(menu==3){
						if(set_mode==0){
							OLED_Clear();
							OLED_Print("Optimise Mode!", 20, 3);
							HLK_LD2410_Enable();
							LD2410_Set_Threshold(2, 100, 100);  	
							HAL_GPIO_WritePin(ESP_EN_PORT, ESP_EN_PIN, GPIO_PIN_SET);
							HAL_Delay(500);
						}
						else if(set_mode==1){
							OLED_Clear();
							OLED_Print("Normal Mode!", 25, 3);
							HLK_LD2410_Enable();
							LD2410_Set_Threshold(2, 60, 60);
							HAL_GPIO_WritePin(ESP_EN_PORT, ESP_EN_PIN, GPIO_PIN_SET);
							HAL_Delay(500);
						}
						else if(set_mode==2){
							OLED_Clear();
							OLED_Print("Power Saving Mode!", 10, 3);
							HLK_LD2410_Enable();
							HLK_LD2410_Disable();
							HAL_GPIO_WritePin(ESP_EN_PORT, ESP_EN_PIN, GPIO_PIN_RESET);
							HAL_Delay(500);
						}
						menu=1; mode=0;
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
							if (setting == 0 || setting == 4 ) { 
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

	void UI_Proccess_Keypad(char key) {
			if (key == 0) return;

			// --- MÀN HÌNH CHỜ ---
			if (menu == 0) {
				// Nếu nhấn D khi ở màn chờ -> tắt đèn (dùng remote ON/OFF)
				if (key == 'D') {
					light_active = 0;
					alarm_active = 0;
					manual_active = 0;
					manual_override_off = 1;
					manual_off_until = HAL_GetTick() + 30000; // block auto-on 30s
					OLED_Clear();
					return;
				}
				if (key == 'A') { mode = 0; menu = 1; OLED_Clear(); } // Vào Menu
				else if (key == 'B') { menu = 2; setting = 0; OLED_Clear(); } // Shortcut vào Setting
					return;
			}

			// --- MÀN HÌNH MENU CHÍNH ---
			if (menu == 1 ) {
					if (key == '2') { mode--; if (mode < 0) mode = 2; OLED_Clear(); }
					else if (key == '8') { mode++; if (mode > 2) mode = 0; OLED_Clear(); }
					Key_Save_Return(key); // Xử lý nhấn '*' để chọn, '#' để thoát
					return;
			}

			// --- MÀN HÌNH SETTING ---
			if (menu == 2) {
					if (setting == 0) { // Đang ở Menu chọn loại Setting
							if (key == '4') { choose = 1; setting = 4; locate = 0; temp_hour = 0; temp_minute = 0; temp_second = 0; OLED_Clear(); }
							else if (key == '6') {
								DS3231 get_alarm;
								DS3231_GetAlarm(&get_alarm);
								temp_hour = get_alarm.hours;
								temp_minute = get_alarm.minutes;
								temp_second = get_alarm.seconds;
								choose = 2; setting = 1; locate = 0; OLED_Clear();
							}
					} 
					else if (setting == 4) { // Đang ở bảng Hướng dẫn, chờ chọn B C D
						
							 if(key=='1') { setting = 5; locate = 0; OLED_Clear(); }
										 if(key=='1') {
											DS3231 current;
											DS3231_GetTime(&current);
											temp_day = current.day;
											temp_date = current.date;
											temp_month = current.month;
											// Force displayed/editable year to 2026
											temp_year = 26;
											setting = 5;
											locate = 0;
											OLED_Clear();
										 }
							 else if(key=='2'){setting = 1; locate =0; OLED_Clear();}
							 else if(key=='#'){setting =0, locate=0; OLED_Clear();}
					} 
					else if (setting >= 5 && setting <= 8) { 
							// ĐANG NHẬP NGÀY THÁNG NĂM
						if (key == 'A') { setting = 5; locate = 0; OLED_Clear(); } 
						else if (key == 'B') { setting = 6; locate = 0; OLED_Clear(); } 
						else if (key == 'C') { setting = 7; locate = 0; OLED_Clear(); } 
						else if (key == 'D') { setting = 8; locate = 0; OLED_Clear(); }
							
						else if ((key >= '0' && key <= '9') || key == '#') {
							if (setting == 5) Handle_SpcKey(key, &temp_day, 7);
							else if (setting == 6) Handle_SpcKey(key, &temp_date, 31);
							else if (setting == 7) Handle_SpcKey(key, &temp_month, 12);
							else if (setting == 8) Handle_SpcKey(key, &temp_year, 99);
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
			if(menu==3){
		if(key=='A'){
			// OPTIMISE MODE: CẢI THIỆN ĐỘ NHẠY VÀ ĐỘ CHÍNH XÁC
			set_mode=0;
			OLED_Clear();
		}
		else if(key=='B'){
			// NORMAL MODE: CHẾ ĐỘ MẶC ĐỊNH CỦA CẢM BIẾN
			set_mode=1;
			OLED_Clear();
		}
		else if(key=='C'){
			// POWER SAVING MODE: GIẢM TẦN SỐ QUÉT ĐỂ TIẾT KIỆM NĂNG LƯỢNG
			set_mode=2;
			OLED_Clear();
		}
		Key_Save_Return(key);
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
					case RM_Play: 	UI_Proccess_Keypad('*'); break;
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
							OLED_Print("WAIT MENU", 36, 1);
							OLED_DrawSelectionBracket(30, 1, 70);
							OLED_Print("A:Menu", 05, 0);
							OLED_Print("B:Setting", 70, 0);
							sprintf(time_str, "%02d:%02d", time.hours, time.minutes);
							OLED_Print(time_str, 45, 2);
							sprintf(date_str, "T%d %02d/%02d/20%02d", time.day, time.date, time.month, time.year);
							OLED_Print(date_str, 25, 3);
							
							OLED_Print("Mode: ", 0, 5); 
							if(set_mode == 0)      OLED_Print("Optimise    ", 40, 5); 
							else if(set_mode == 1) OLED_Print("Normal      ", 40, 5);
							else if(set_mode == 2) OLED_Print("Power Saving", 40, 5);
					if (alarm_set == 1) {
						DS3231 alarm_info;
						DS3231_GetAlarm(&alarm_info);
						DS3231 time_now;
						DS3231_GetTime(&time_now);
						char alarm_str[30], time_str2[20];
						sprintf(alarm_str, "Alm:%02d:%02d:%02d", alarm_info.hours, alarm_info.minutes, alarm_info.seconds);
						sprintf(time_str2, "Now:%02d:%02d:%02d", time_now.hours, time_now.minutes, time_now.seconds);
						OLED_Print(alarm_str, 0, 4);
						OLED_Print(time_str2, 0, 7);
						HAL_Delay(50);
					}
							break;
							
					case 1: // MÀN HÌNH MENU CHÍNH

							OLED_DrawSelectionBracket(40, 0, 50);
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
									OLED_Print("4: Chinh Ngay/Timer", 10, 2);
									OLED_Print("6: Chinh Alarm", 10, 4);
									OLED_Print("#: Quay lai", 0, 7);
							}
							else if (setting == 4) {
									OLED_Print("--- CHON MUC ---", 15, 0); 
                                OLED_DrawSelectionBracket(15, 0, 100);
                                OLED_Print("1: Chinh Ngay Thang", 5, 2);
                                OLED_Print("2: Chinh Gio Phut ", 5, 4);

                                OLED_Print("#: Quay lai", 0, 7);
							}
							else if(setting == 9){
									OLED_Print("--- CHON MUC ---", 15, 0); 
									OLED_Print("B:Gio C:Phut D:Giay", 0, 3);
							}
							else if (setting >= 5 && setting <= 8) { 
									// GIAI ĐOẠN 1: VẼ MÀN HÌNH CÀI NGÀY
									OLED_Print("NGAY THANG", 10, 0); 

									char buf_d[20], buf_m[20], buf_y[20],buf_day[20];
									sprintf(buf_day, "%s THU : %02d", (setting == 5 ? "*" : " "), temp_day);
									sprintf(buf_d, "%s NGAY : %02d", (setting == 6 ? "*" : " "), temp_date);     
									sprintf(buf_m, "%s THANG: %02d", (setting == 7 ? "*" : " "), temp_month);
									sprintf(buf_y, "%s NAM  : 20%02d", (setting == 8 ? "*" : " "), temp_year);
									
									OLED_Print(buf_day, 15, 2);
									OLED_Print(buf_d, 15, 3); 
									OLED_Print(buf_m, 15, 4); 
									OLED_Print(buf_y, 15, 5); 
									
									OLED_Print("*:Next      #:Xoa", 0, 7);
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
							OLED_Print("A: Optimise", 10, 2);
							OLED_Print("B: Normal", 10, 4);
							OLED_Print("C: Power Saving", 10, 6);

							if(set_mode == 0) OLED_DrawSelectionBox(10, 2, 100);
							else if(set_mode == 1) OLED_DrawSelectionBox(10, 4, 90);
							else if(set_mode == 2) OLED_DrawSelectionBox(10, 6, 120);
							break;
							
					case 4: // MÀN HÌNH RADAR
							if (is_bg_drawn == 0) {
									OLED_Clear(); 
									OLED_Print("RADAR", 0, 0);
									OLED_Print("#:Out", 95, 0);
									OLED_Print("Status:", 0, 2);
									OLED_Print("Moving:", 0, 4);
									OLED_Print("Static:", 0, 6);
									is_bg_drawn = 1;
							}

							char txt_buffer[20];

							if (Radar.state == 0)      OLED_Print("CLEAR   ", 55, 2); 
							else if (Radar.state == 1) OLED_Print("MOVING  ", 55, 2);
							else if (Radar.state == 2) OLED_Print("STATIC  ", 55, 2);
							else if (Radar.state == 3) OLED_Print("MOV+STA ", 55, 2);

							if (Radar.state == 1 || Radar.state == 3) {
									sprintf(txt_buffer, "%-4d cm", Radar.moving_dist);
									OLED_Print(txt_buffer, 55, 4);
							} else {
									OLED_Print("---     ", 55, 4); 
							}

							if (Radar.state == 2 || Radar.state == 3) {
									sprintf(txt_buffer, "%-4d cm", Radar.static_dist);
									OLED_Print(txt_buffer, 55, 6);
							} else {
									OLED_Print("---     ", 55, 6);
							}
							
							HAL_Delay(50); 
							break;
		}
	}

void UI_System_Process(void) {
    DS3231_GetTime(&time); // Luôn cập nhật thời gian thực

    // 1. KIỂM TRA BÁO THỨC (ALARM)
    if (alarm_set == 1) {
        DS3231 alarm_info;
        DS3231_GetAlarm(&alarm_info);
        
        if (alarm_info.hours == time.hours && 
            alarm_info.minutes == time.minutes && 
			alarm_info.seconds == time.seconds) 
        {
			// Toggle light state at the scheduled time.
			if (light_active == 1) {
				light_active = 0;
				alarm_active = 0;
				manual_active = 0;
				manual_override_off = 0;
				manual_off_until = 0;
			} else {
				light_active = 1;
				alarm_active = 1;
				manual_active = 0;
				manual_override_off = 0;
				manual_off_until = 0;
				timer_radar = HAL_GetTick();
			}
			alarm_set = 0; // clear saved alarm so it won't retrigger
        }
    }

    // 2. KIỂM TRA RADAR (SMART LIGHT)
	// if manual override off is active, check expiration
	if (manual_override_off) {
		if (HAL_GetTick() >= manual_off_until) {
			manual_override_off = 0; // allow auto-on again
		}
	}

	if (!manual_override_off) {
		if (Radar.state != 0 && set_mode != 2) {
			light_active = 1;
			timer_radar = HAL_GetTick(); 
		}
	}

    // 3. ĐIỀU KHIỂN CHÂN ĐÈN (LED_PIN)
    if (light_active == 1) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);   
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Onboard LED ON (Active Low)

		// Nếu bật bằng web (manual) hoặc alarm, giữ tới khi có lệnh tắt khác
		if (manual_active || alarm_active) {
			// nothing: keep light_active == 1 until turned off explicitly
		} else {
			// Tự động tắt sau 5 giây nếu không còn phát hiện người (radar)
			if (Radar.state == 0) {
				if (HAL_GetTick() - timer_radar >= 5000) {
					light_active = 0;
				}
			}
		}
    } 
    else {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); 
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // Onboard LED OFF
    }

    // 4. ĐIỀU KHIỂN NGUỒN ESP (ESP_EN_PIN) - CHỈ TẮT KHI Ở CHẾ ĐỘ POWER SAVING (set_mode == 2)
    if (set_mode == 2) {
        HAL_GPIO_WritePin(ESP_EN_PORT, ESP_EN_PIN, GPIO_PIN_RESET); // Tắt ESP để tiết kiệm điện
    } else {
        HAL_GPIO_WritePin(ESP_EN_PORT, ESP_EN_PIN, GPIO_PIN_SET);   // Luôn bật ESP để duy trì kết nối Web
    }
}

void UI_ESP_Process(void) {
	if(Rx_Flag)
	{
		if(u8_RxBuff[0]=='T')
		{
			// Extract digits only for HH:MM:SS
			uint8_t digits[6];
			int digit_idx = 0;
			for(int i = 1; i < 20 && digit_idx < 6; i++) {
				if(u8_RxBuff[i] >= '0' && u8_RxBuff[i] <= '9') {
					digits[digit_idx++] = u8_RxBuff[i] - '0';
				}
			}
			if(digit_idx == 6) {
				uint8_t h = digits[0] * 10 + digits[1];
				uint8_t m = digits[2] * 10 + digits[3];
				uint8_t s = digits[4] * 10 + digits[5];
				DS3231 set_timer;
				DS3231_GetTime(&set_timer);
				set_timer.hours = h;
				set_timer.minutes = m;
				set_timer.seconds = s;
				DS3231_SetTime(&set_timer);
				// Read back and display stored time to verify RTC write
				DS3231_GetTime(&set_timer);
				{
					char buf[30];
					sprintf(buf, "Now:%02d:%02d:%02d Y:20%02d", set_timer.hours, set_timer.minutes, set_timer.seconds, set_timer.year);
					OLED_Clear();
					OLED_Print("Timer Updated!", 15, 2);
					OLED_Print(buf, 5, 4);
					HAL_Delay(800);
					OLED_Clear();
				}
			}
		}
		else if(u8_RxBuff[0]=='A')
		{
			// Extract digits only for HH:MM:SS
			uint8_t digits[6];
			int digit_idx = 0;
			for(int i = 1; i < 20 && digit_idx < 6; i++) {
				if(u8_RxBuff[i] >= '0' && u8_RxBuff[i] <= '9') {
					digits[digit_idx++] = u8_RxBuff[i] - '0';
				}
			}
			if(digit_idx == 6) {
				uint8_t h = digits[0] * 10 + digits[1];
				uint8_t m = digits[2] * 10 + digits[3];
				uint8_t s = digits[4] * 10 + digits[5];
				DS3231 set_alarm;
				DS3231_GetAlarm(&set_alarm);
				set_alarm.hours = h;
				set_alarm.minutes = m;
				set_alarm.seconds = s;
				DS3231_SetAlarm(&set_alarm);
				HAL_Delay(100);

				DS3231_GetAlarm(&set_alarm);
				temp_hour = set_alarm.hours;
				temp_minute = set_alarm.minutes;
				temp_second = set_alarm.seconds;

				OLED_Clear();
				alarm_set=1;
			}
		}
else if(u8_RxBuff[0] == 'E') 
{
    uint8_t digits[8];
    int digit_idx = 0;

    for(int i = 1; i < 30 && digit_idx < 8; i++) { 
        if(u8_RxBuff[i] == '\0') break; 
        
        if(u8_RxBuff[i] >= '0' && u8_RxBuff[i] <= '9') {
            digits[digit_idx++] = u8_RxBuff[i] - '0';
        }
    }
    
    if(digit_idx == 8) {
        uint8_t d = digits[0] * 10 + digits[1];
        uint8_t m = digits[2] * 10 + digits[3];
        

        uint8_t y = digits[6] * 10 + digits[7]; 
        
        DS3231 set_date;
        DS3231_GetTime(&set_date); 
                set_date.date = d;
        set_date.month = m;
        set_date.year = y; 
        
        DS3231_SetTime(&set_date);

        DS3231_GetTime(&set_date);
        char bufd[30];
        sprintf(bufd, "D:%02d/%02d/20%02d", set_date.date, set_date.month, set_date.year);
        OLED_Clear();
        OLED_Print("Date Updated!", 15, 2);
        OLED_Print(bufd, 5, 4);
        HAL_Delay(800);
        OLED_Clear();
    }
}
		else if(u8_RxBuff[0]=='D')
		{

			int found = -1;
			for (int i = 1; i < sizeof(u8_RxBuff); i++) {
				char c = u8_RxBuff[i];
				if (c == '1') { found = 1; break; }
				if (c == '0') { found = 0; break; }
				if (c == '\0') break;
			}
			if (found == 1) {
				light_active = 1;
				timer_radar = HAL_GetTick(); 
				alarm_active = 0; 
				manual_active = 1;
				manual_override_off = 0; 
			} else if (found == 0) {
				light_active = 0; // Tắt thủ công
				alarm_active = 0; // nếu đang ở chế độ báo thức, hủy nó khi tắt thủ công
				manual_active = 0; // clear manual flag
				
				manual_override_off = 1;
				manual_off_until = HAL_GetTick() + 30000; 
			}
		}

		memset(u8_RxBuff, 0, sizeof(u8_RxBuff));
        RxIndex = 0;
        Rx_Flag = 0;
	}
}
