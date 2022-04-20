#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include "FS.h"
#include <ArduinoJson.h> //ArduinoJSON6

WiFiClient espClient;
PubSubClient client(espClient);
const IPAddress apIP(192, 168, 1, 1);
const char* apSSID = "ESP_SerialToMQTT";
boolean settingMode = false;
String ssidList;
DNSServer dnsServer;
ESP8266WebServer webServer(80);
const char *ssid, *password, *mqtt_server, *mqtt_user, *mqtt_pass, *topic_UP, *topic_DOWN, *HostName;
DynamicJsonDocument WIFI(2048);

char LectChar[50];
String Lect;

void callback(String topic, byte* message, unsigned int length) {
  if (topic == topic_DOWN) {
    String messageTemp;
    for (int i = 0; i < length; i++) {
      messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  if (load_wifi()) {
    if (setup_wifi()) {
      settingMode = false;
      WiFi.mode(WIFI_STA);
      client.setServer(mqtt_server, 1883);
      client.setCallback(callback);
      return;
    }
  }
  settingMode = true;
  setupMode();
}

void loop() {

  if (settingMode) {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
  else {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }

  if (Serial.available()) {
    Lect = Serial.readString();
    if (Lect[0] == 'o') {
      Lect.toCharArray(LectChar, 50);
      client.publish(topic_UP, LectChar);
    }
    Serial.read();
  }
  delay(0);
}
