#include "wifiConfig.h"

void setup() {
  // 1. Khởi tạo Serial để debug (khớp với baudrate bạn dùng)
  Serial.begin(115200);

  // 2. Thiết lập chân IO (Các biến ledPin, btnPin đã có trong file cpp/h)
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);

  // 3. Khởi tạo bộ nhớ EEPROM
  EEPROM.begin(128);

  // 4. Gọi hàm phát WiFi mặc định (Đã sửa trong wifiConfig)
  setupWifi();

  // 5. Khởi tạo Web Server và các Route (setTime, setAlarm, device)
  // wifiMode sẽ được set là 1 trong setupWifi()
  setupWebServer();
  
  Serial.println("He thong da san sang!");
}

void loop() {
  // Kiểm tra nút nhấn (reset EEPROM nếu nhấn giữ)
  checkButton();

  // Luôn xử lý các yêu cầu từ Web Browser gửi đến
  webServer.handleClient();

  // Cập nhật trạng thái nháy LED theo wifiMode
  ledControl();
}