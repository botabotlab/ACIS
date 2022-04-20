//Dependencies
//--------------------------------------
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include "FS.h"
#include <ArduinoJson.h> //ArduinoJSON6
#include <Adafruit_NeoPixel.h>
//--------------------------------------

//WiFi Settings
//----------------------------------------------------------------------------------------
const IPAddress apIP(192, 168, 1, 1);
const char* apSSID = "Microscope_SK6812";
boolean SettingMode = false;
String ssidList;
int maxtry = 30;
DNSServer dnsServer;
ESP8266WebServer webServer(80);
const char *ssid, *password, *mqtt_server, *mqtt_user, *mqtt_pass, *topic_led, *topic_all, *HostName;
DynamicJsonDocument WIFI(2048);

WiFiClient espClient;
PubSubClient client(espClient);
bool WiFiConnected = false;
//----------------------------------------------------------------------------------------

//#define FASTLED_INTERNAL

#define NUM_LEDS 24
#define LED_PIN 14
Adafruit_NeoPixel LED(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);
//CRGB LED[NUM_LEDS];

DynamicJsonDocument CMD(2048);
DynamicJsonDocument STATE(2048);

int MatrixPos[][6] = {
  {2,1,0,23,22,21},
  {3,4,5,18,19,20},
  {8,7,6,17,16,15},
  {9,10,11,12,13,14},
};

//MQTT
//-------------------------------------------------------------------
void MQTT_loop() {
  if (WiFiConnected == true) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
}

void callback(String topic, byte* message, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.print("Topic : ");
  Serial.println(topic);
  Serial.print("Payload : ");
  Serial.println(messageTemp);
  Settings(topic, messageTemp);
}
//-------------------------------------------------------------------

//LED Settings
//-------------------------------------------------
void Settings(String topic, String messageTemp) {
  deserializeJson(CMD, messageTemp);
  int X = 0, Y = 0, R = 0, G = 0, B = 0, W = 0;
  if (CMD.containsKey("R")) {
    R = CMD["R"];
  }
  if (CMD.containsKey("G")) {
    G = CMD["G"];
  }
  if (CMD.containsKey("B")) {
    B = CMD["B"];
  }
  if (CMD.containsKey("W")) {
    W = CMD["W"];
  }
  if (CMD.containsKey("X")) {
    X = CMD["X"];
  }
  if (CMD.containsKey("Y")) {
    Y = CMD["Y"];
  }
  if (topic == topic_led) {
    //LED[MatrixPos[Y][X]] = CRGB( R, G, B);
    LED.setPixelColor(MatrixPos[Y][X], LED.Color(R, G, B , W));
    STATE[MatrixPos[Y][X]]["R"] = R;
    STATE[MatrixPos[Y][X]]["G"] = G;
    STATE[MatrixPos[Y][X]]["B"] = B;
    STATE[MatrixPos[Y][X]]["W"] = W;
    save_state();
  }
  if (topic == topic_all) {
    //fill_solid(LED, NUM_LEDS, CRGB( R, G, B));
    LED.fill(LED.Color(R, G, B , W), 0, NUM_LEDS);
    for (int i = 0; i < 25; i++) {
      STATE[i]["R"] = R;
      STATE[i]["G"] = G;
      STATE[i]["B"] = B;
      STATE[i]["W"] = W;
    }
    save_state();
  }
  //FastLED.show();
  LED.show();
}

//-------------------------------------------------
void setup() {
  //FastLED.addLeds<WS2811, DATA_PIN, GRB>(LED, NUM_LEDS);
  LED.begin();
  Serial.begin(115200);
  Serial.println();
  Serial.setTimeout(500);

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  load_wifi();
  WiFiConnected = setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  load_state();
  restore_state();
}

void loop() {
  MQTT_loop();
  if (Serial.available()) {
    String input = Serial.readString();
    String Select = input.substring(0, 1);
    String Command = input.substring(1);

    //HELP
    //....................................
    if (Select == "H" || Select == "h") {
      Serial.println("-------------------------------------------------------------");
      Serial.println("Available commands :");
      Serial.println("H or h : Help");
      Serial.println();
      Serial.println("R or r : Start a Web UI to edit de WiFi/MQTT settings");
      Serial.println();
      Serial.println("W or w + JSON: Edit the WiFi/MQTT With the provided JSON");
      Serial.println("Example : W {\"SSID\": \"WiFi SSID\", \"Pass\":\"WiFi Pass\", \"MQTT_Server\":\"Broker.IP\", \"MQTT_Pass\":\"Broker.Pass\", \"Topic_LED1\":\"topic/led/01\", \"Topic_LED2\":\"topic/led/02\", \"HostName\":\"HostName\"}");
      Serial.println();
      Serial.println("A or a + JSON : Control the whole box");
      Serial.println("A {\"R\":0-255, \"G\":0-255, \"B\":0-255, \"W\":0-255 }");
      Serial.println("Example : A {\"R\":255, \"G\":0, \"B\":0, \"W\":0 }");
      Serial.println();
      Serial.println("XYRGB JSON : Control one LED");
      Serial.println("{\"X\": 0-4, \"Y\": 0-4, \"R\":0-255, \"G\":0-255, \"B\":0-255, \"W\":0-255 }");
      Serial.println("Example : {\"X\": 2, \"Y\": 2, \"R\":255, \"G\":0, \"B\":0, \"W\":0 }");

      Serial.println();
      Serial.println("0 : Turn OFF Matrix");
      Serial.println("-------------------------------------------------------------");
    }
    //....................................

    //RESET WiFI Settings and start web GUI
    //....................................
    if (Select == "R" || Select == "r") {
      SettingMode = true;
      SetupMode();
    }
    //....................................

    //Edit WiFi Settings from Serial
    //.............................................
    if (Select == "W" || Select == "w") {
      deserializeJson(CMD, Command);
      if (CMD.containsKey("SSID")) {
        WIFI["SSID"] = CMD["SSID"];
      }
      if (CMD.containsKey("Pass")) {
        WIFI["Pass"] = CMD["Pass"];
      }
      if (CMD.containsKey("MQTT_Server")) {
        WIFI["MQTT_Server"] = CMD["MQTT_Server"];
      }
      if (CMD.containsKey("MQTT_Pass")) {
        WIFI["MQTT_Pass"] = CMD["MQTT_Pass"];
      }
      if (CMD.containsKey("Topic")) {
        WIFI["Topic"] = CMD["Topic"];
      }
      if (CMD.containsKey("HostName")) {
        WIFI["HostName"] = CMD["HostName"];
      }
      save_wifi();
    }
    //.............................................

    //Serial Control one LED
    //................................
    if (Select == "{") {
      Settings(topic_led, input);
      Serial.print("CMD : ");
      Serial.println(input);
    }
    //................................

    //Serial Control ALL
    //................................
    if (Select == "A" || Select == "a") {
      Settings(topic_all, Command);
      Serial.print("CMD : ");
      Serial.println(Command);
    }
    //................................

    //Serial Control ALL
    //................................
    if (Select == "0") {
      Settings(topic_all, "{\"R\":0,\"G\":0,\"B\":0,\"W\":0}");
      Serial.print("CMD : ");
      Serial.println(Command);
    }
    //................................

    //Serial Control ALL
    //................................
    if (Select == "S" || Select == "s") {
      restore_state();
    }
    //................................

    Serial.read();
  }
}
