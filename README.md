# TIMER COUNTER
<p align="left">
  <img src="https://img.shields.io/badge/Main%20MCU-STM32F103C8T6-blue?style=flat&logo=stmicroelectronics" />
  <img src="https://img.shields.io/badge/Wireless-ESP8286--01-red?style=flat&logo=espressif" />
  <img src="https://img.shields.io/badge/IDE-Keil%20C%20%2F%20CubeMX-orange?style=flat&logo=arm" />
</p>

---
### 📂 Chi tiết thư mục
<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/fire.png" width="100%">
<details>
<summary><b>📂 Bấm để xem cấu trúc thư mục dự án (Project Structure)</b></summary>

```text
📂 Timer Counter
├── 📦 Core
│   ├── 📂 Inc
│   │   ├── 📜 DS1307.h        # Thư viện RTC DS3231/DS1307
│   │   ├── 📜 Esp_01.h        # Thư viện giao tiếp ESP-01
│   │   ├── 📜 FontSSD1306.h   # Font chữ cho OLED SSD1306
│   │   ├── 📜 HLK_LD2410.h    # Driver cảm biến Radar hiện diện
│   │   ├── 📜 Keypad.h        # Thư viện ma trận phím
│   │   ├── 📜 Menu_UI.h       # Logic Menu & Giao diện OLED
│   │   ├── 📜 MH-R38.h        # Giải mã tín hiệu Remote IR
│   │   └── 📜 SSD1306.h       # Thư viện điều khiển màn hình OLED
│   └── 📂 Src
│       ├── 📜 main.c          # Luồng xử lý chính
│       ├── 📜 Menu_UI.c       # Cấu trúc hiển thị Menu
│       └── 📜 ...             # Các file xử lý tương ứng (.c)
├── 📦 Esp-01
│   ├── 📂 inc
│   │   └── 📜 wifiConfig.h    # Cấu hình WiFi & Web Server
│   └── 📂 src
│       └── 📜 WifiConfig.cpp  # Xử lý logic kết nối
└── 📦 PCB
    ├── 📂 GiaCong             # File Gerber, BOM để đặt mạch
    └── 📂 Timer_Counter       # File thiết kế gốc (KiCad)
 ```
 </details>

### 🛠️ Phần cứng sử dụng
<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" width="100%">
**1. Vi điều khiển trung tâm**
* STM32F103C8T6 (ARM Cortex-M3)

**2. Cảm biến & Module ngoại vi**
* **Mô-đun kết nối:** ESP8266 (ESP-01) cấu hình Web Server.
* **Cảm biến hiện diện:** Radar HLK-LD2410C (nhận diện vi chuyển động).
* **Module thời gian thực:** DS3231 / DS1307 (lưu giữ ngày giờ).
* **Màn hình hiển thị:** OLED 0.96 inch (IC SSD1306).

* **Khối điều khiển & Giao tiếp:** Bàn phím ma trận (Keypad) & MH-R38 (nhận tín hiệu Remote IR)

## Sơ đồ nối chân 
| 🛠️ Phần cứng | 📌 Chân STM32 | 🔗 Giao tiếp | 📝 Chức năng |
| :--- | :---: | :---: | :--- |
| 🖥️ **OLED SSD1306** | `PB6 (SCL)`<br>`PB7 (SDA)` | `I2C1` | Hiển thị thông tin giao diện người dùng |
| 🌐 **ESP-01 (WiFi)** | `PA9 (TX)`<br>`PA10 (RX)` | `UART1` | Web Server & Phát WiFi điều khiển từ xa |
| ⏳ **RTC DS3231** | `PB6 (SCL)`<br>`PB7 (SDA)` | `I2C1` | Lưu thời gian & Báo thức (chạy khi mất điện) |
| 📡 **Radar LD2410** | `PA2 (TX)`<br>`PA3 (RX)` | `UART2` | Nhận diện người (vi chuyển động) |
| 🎛️ **Keypad 4x4** | `PB12-15`<br>`PA4-7` | `GPIO` | Điều khiển Menu |
| 🖲️ **Mắt thu IR** | `PA1` | `TIM2` | Nhận tín hiệu Remote hồng ngoại

## Yêu Cầu
<p align="center">
  <img src="https://img.shields.io/badge/STM32-0099FF.svg?style=flat&logo=stmicroelectronics&logoColor=white" />
  <img src="https://img.shields.io/badge/C-0066FF.svg?style=flat&logo=c&logoColor=white" />
  <img src="https://img.shields.io/badge/KiCad-3300CC.svg?style=flat&logo=kicad&logoColor=white" />
  <img src="https://img.shields.io/badge/ESP8266-FF3333?style=flat&logo=espressif&logoColor=white" />
  <img src="https://img.shields.io/badge/Keil%20C-339900.svg?style=flat&logo=arm&logoColor=white" />
  <img src="https://img.shields.io/badge/STM32CubeMX-0099FF.svg?style=flat&logo=stmicroelectronics&logoColor=white" />
  
  <br/><br/>


<p align="center">
  <img src="https://img.shields.io/github/stars/AOD-gm?style=for-the-badge&color=yellow&label=Stars" />
  <img src="https://img.shields.io/github/followers/AOD-gm?style=for-the-badge&color=blue&label=Followers" />
  <img src="https://img.shields.io/badge/Repos-5-orange?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Learning-green?style=for-the-badge" />
</p>