# ĐỒ ÁN VI XỬ LÝ - VI ĐIỀU KHIỂN (TIMER COUNTER)
## THƯ VIỆN
<details>
<summary><b>📂 Timer Counter</b></summary>

> <br> 📦 **Core**
> <br> ┣ 📂 **Inc**
> <br> ┃ ┣ 📜 DS1307.h  -***Thư viện DS3231***
> <br> ┃ ┣ 📜 Esp_01.h  -***Thư viên ESP01***
> <br> ┃ ┣ 📜 FontSSD1306.h-***Thư viện font chữ của SSD1306***
> <br> ┃ ┣ 📜 HLK_LD2410.h  -***Thư viện HLK_LD2410c***
> <br> ┃ ┣ 📜 Keypad.h      -***Thư viện keypad***
> <br> ┃ ┣ 📜 main.h
> <br> ┃ ┣ 📜 Menu_UI.h -***Thư viện Logic code và trình bày trên SSD1306***
> <br> ┃ ┣ 📜 MH-R38.h  -***Thư viện quét IR remote***
> <br> ┃ ┗ 📜 SSD1306.h -***Thư viện SSD1306***
> <br> ┗ 📂 **Src**
> <br> ┃    ┣ 📜 DS1307.c
> <br> ┃    ┣ 📜 Esp_01.c
> <br> ┃    ┣ 📜 FontSSD1306.c
> <br> ┃    ┣ 📜 HLK_LD2410.c
> <br> ┃    ┣ 📜 Keypad.c
> <br> ┃    ┣ 📜 main.c
> <br> ┃    ┣ 📜 Menu_UI.c
> <br> ┃    ┣ 📜 MH-R38.c
> <br> ┗ 📜 SSD1306.c
> <br> 📦 **Esp-01**
> <br> ┣ 📂 **inc**
> <br> ┃ ┗ 📜 wifiConfig.h -***Thư viện Wifi ESP 01***
> <br> ┗ 📂 **src**
> <br> &nbsp;&nbsp;&nbsp;┗ 📜 WifiConfig.cpp
> <br> 📦 **PCB**
> <br> ┣ 📂 **GiaCong**
> <br> ┗ 📂 **Timer_Counter**

</details>

## 🛠️ Phần cứng sử dụng

**1. Vi điều khiển trung tâm**
* STM32F103C8T6 (ARM Cortex-M3)

**2. Cảm biến & Module ngoại vi**
* **Mô-đun kết nối:** ESP8266 (ESP-01) cấu hình Web Server.
* **Cảm biến hiện diện:** Radar HLK-LD2410C (nhận diện vi chuyển động).
* **Module thời gian thực:** DS3231 / DS1307 (lưu giữ ngày giờ).
* **Màn hình hiển thị:** OLED 0.96 inch (IC SSD1306).

* **Khối điều khiển & Giao tiếp:** Bàn phím ma trận (Keypad) & MH-R38 (nhận tín hiệu Remote IR)

## Sơ đồ nối chân 
| Phần cứng     | Chân Stm32    | Giao tiếp  | Chức năng |
|:-------------:|:-------------:| :-----:    |-----  |
|      SSD1306         | PB6(SCL) / PB7(SDA) | I2C1     | Hiển thị thông tin
| ESP-01    | PA9(TX)/PA10(RX)     |   UART1    |      Hiển thị web và phát wifi điều khiển từ xa    |
| DS3231 | PB6(SCL) / PB7(SDA)   |    I2C1 |  Lưu thời gian và báo thức(chạy ngay cả khi ko có điện) |
Radar LD2410   | PA2 / PA3    | UART2        | Nhận diện người
Keypad 4x4     | PB12-15/PA4-7 | GPIO         | Điều khiển Menu
Mắt thu IR     | PA1           |     TIM2     | Remote IR
