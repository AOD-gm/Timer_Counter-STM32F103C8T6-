#include "wifiConfig.h"

// ====== GLOBAL ======
String ssid;
String password;

int wifiMode = 0;

ESP8266WebServer webServer(80);
Ticker blinker;

int ledPin = 2;
int btnPin = 0;

unsigned long lastTimePress = 0;
unsigned long blinkTime = 0;

#define PUSHTIME 5000
// ====== HTML ======
const char html[] PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP Control Panel</title>
    <style>
        body { font-family: Arial; text-align: center; background: #f0f0f0; padding: 20px; }
        .container { background: white; padding: 20px; border-radius: 12px; display: inline-block; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
        .section { margin: 15px 0; padding: 10px; border-bottom: 1px solid #eee; }
        input { padding: 8px; width: 150px; text-align: center; border: 1px solid #ccc; border-radius: 4px; }
        button { padding: 10px 20px; margin: 5px; border: none; border-radius: 5px; cursor: pointer; font-weight: bold; }
        .btn-set { background: #007bff; color: white; }
        .btn-on { background: #28a745; color: white; }
        .btn-off { background: #dc3545; color: white; }
    </style>
</head>
<body>
    <div class="container">
        <h2>MODE: CONTROL</h2>
        
        <div class="section">
            <p><strong>SET TIME</strong> (hh:mm:ss)</p>
            <input type="text" id="timeInput" placeholder="12:00:00">
            <button class="btn-set" onclick="send('/setTime', 'timeInput')">SET</button>
        </div>

        <div class="section">
            <p><strong>SET ALARM</strong> (hh:mm:ss)</p>
            <input type="text" id="alarmInput" placeholder="06:30:00">
            <button class="btn-set" onclick="send('/setAlarm', 'alarmInput')">SET</button>
        </div>

        <div class="section">
            <button class="btn-on" onclick="call('/device?state=1')">BẬT THIẾT BỊ</button>
            <button class="btn-off" onclick="call('/device?state=0')">TẮT THIẾT BỊ</button>
        </div>
        
        <button onclick="call('/reStart')" style="background: #6c757d; color: white;">RESTART ESP</button>
    </div>

    <script>
        function send(path, id) {
            var val = document.getElementById(id).value;
            if(!val) return alert("Chưa nhập thông số!");
            var x = new XMLHttpRequest();
            x.open("GET", path + "?val=" + val, true);
            x.send();
            alert("Đã gửi: " + val);
        }
        function call(path) {
            var x = new XMLHttpRequest();
            x.open("GET", path, true);
            x.send();
        }
    </script>
</body>
</html>
)html";

// ===== EEPROM =====
void writeStringToEEPROM(int addr, String data) {
  for (int i = 0; i < data.length(); i++) {
    EEPROM.write(addr + i, data[i]);
  }
  EEPROM.write(addr + data.length(), '\0');
}

String readStringFromEEPROM(int addr, int maxLen) {
  char data[maxLen];
  for (int i = 0; i < maxLen; i++) {
    data[i] = EEPROM.read(addr + i);
    if (data[i] == '\0') break;
  }
  return String(data);
}

// ===== LED =====
void blinkLed(uint32_t t) {
  if (millis() - blinkTime > t) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    blinkTime = millis();
  }
}

void ledControl() {
  if (digitalRead(btnPin) == LOW) {
    if (millis() - lastTimePress < PUSHTIME) blinkLed(1000);
    else blinkLed(50);
  } else {
    if (wifiMode == 0) blinkLed(50);
    else if (wifiMode == 1) blinkLed(3000);
    else if (wifiMode == 2) blinkLed(300);
  }
}

// ===== WIFI EVENT =====
void onStationGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.print("STA IP: ");
  Serial.println(WiFi.localIP());
  wifiMode = 1;
}

void onStationDisconnected(const WiFiEventStationModeDisconnected& event) {
  wifiMode = 2;
}

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

// ===== WIFI =====
void setupWifi() {
  // 1. Dọn dẹp các kết nối cũ để đảm bảo ổn định
  WiFi.softAPdisconnect(true);
  WiFi.disconnect(true);
  
  WiFi.mode(WIFI_AP); 

  // Tạo tên AP dựa trên địa chỉ MAC (ví dụ: ESP-8CAA)
  String mac = WiFi.softAPmacAddress();
  String apName = "ESP-" + mac.substring(mac.length() - 5);
  apName.replace(":", ""); 

  // Phát WiFi không mật khẩu (NULL) để bạn kết nối nhanh nhất có thể
  if (WiFi.softAP(apName.c_str(), NULL)) {
    Serial.println("-----------------------");
    Serial.println("CHẾ ĐỘ MẶC ĐỊNH: TỰ PHÁT WIFI");
    Serial.print("Tên WiFi: "); Serial.println(apName);
    Serial.print("Địa chỉ IP: "); Serial.println(WiFi.softAPIP()); // Mặc định là 192.168.4.1
    Serial.println("-----------------------");
    
    wifiMode = 1; // Đánh dấu đang ở chế độ AP để loop() xử lý WebServer
  }

  // 3. (Vẫn giữ lại việc đọc EEPROM để dùng khi cần thiết trong code)
  ssid = readStringFromEEPROM(0, 32);
  password = readStringFromEEPROM(32, 64);
  ssid.trim();
  password.trim();
}
// ===== WEB =====
void setupWebServer() {
  // Trang chủ hiển thị giao diện
  webServer.on("/", [] {
    webServer.send_P(200, "text/html", html);
  });

  // Xử lý dữ liệu từ trang web gửi về
  webServer.on("/setTime", handleSetTime);
  webServer.on("/setAlarm", handleSetAlarm);
  webServer.on("/device", handleDevice);

  webServer.on("/reStart", [] {
    webServer.send(200, "text/plain", "Restarting...");
    delay(1000);
    ESP.restart();
  });

  webServer.begin();
  Serial.println("Web Server đã khởi động!");
}
// ===== BUTTON =====
void checkButton() {
  if (digitalRead(btnPin) == LOW) {
    if (millis() - lastTimePress > PUSHTIME) {
      for (int i = 0; i < 100; i++) EEPROM.write(i, 0);
      EEPROM.commit();
      ESP.restart();
    }
  } else {
    lastTimePress = millis();
  }
}
//===========SETDATA===================
void handleSetTime() {
  if (webServer.hasArg("val")) {
    String timeData = webServer.arg("val"); 
    
    Serial.print("T:");
    Serial.print(timeData);
    Serial.print("\r");
    
    webServer.send(200, "text/plain", "Time Updated");
  }
}

void handleSetAlarm() {
  if (webServer.hasArg("val")) {
    String alarmData = webServer.arg("val");

    Serial.print("A:");
    Serial.print(alarmData);
    Serial.print("\r");
    
    webServer.send(200, "text/plain", "Alarm Updated");
  }
}

void handleDevice() {
  if (webServer.hasArg("state")) {
    String state = webServer.arg("state"); 
    
    Serial.print("D:");
    Serial.print(state);
    Serial.print("\r"); 

    webServer.send(200, "text/plain", "OK");
  }
}