#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
const char* mqtt_server = "";
const char* device_id   = ""; 
const char* mqtt_user   = "";
const char* mqtt_pass   = ""; 

WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(device_id, mqtt_user, mqtt_pass)) {
      String topic = String("devices/") + String(device_id) + String("/messages/devicebound/#");
      client.subscribe(topic.c_str());
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("Setup");

  espClient.setInsecure(); 
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
