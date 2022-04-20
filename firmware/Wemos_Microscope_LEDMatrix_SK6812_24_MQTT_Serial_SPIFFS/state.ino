//---------------------------------------------------------
boolean load_state() {
  File config_file = SPIFFS.open("/state.json", "r");
  if (!config_file) {
    Serial.println("Failed to open file for reading");
    return false;
  }
  String  config_string = config_file.readString();
  deserializeJson(STATE, config_string);
  config_file.close();

  Serial.println("State JSON loaded");
  return true;
}
//---------------------------------------------------------

//---------------------------------------------------------
void save_state() {
  String config_string;
  //Open the config.json file (Write Mode)
  File config_file = SPIFFS.open("/state.json", "w");
  if (!config_file) {
    Serial.println("Failed to open file (Writing mode)");
    return;
  }
  serializeJson(STATE, config_string);
  //Save and close the JSON file
  if (config_file.println(config_string)) {
    //Serial.println("New Config");
    //Serial.println(config_string);
  } else {
    Serial.println("File write failed");
  }
  config_file.close();
}
//---------------------------------------------------------

void restore_state() {
  for (int i = 0; i < 25; i++) {
    int R = STATE[i]["R"];
    int G = STATE[i]["G"];
    int B = STATE[i]["B"];
    int W = STATE[i]["W"];
    //LED[i] = CRGB( STATE[i]["R"], STATE[i]["G"], STATE[i]["B"]);
    LED.setPixelColor(i, LED.Color(R, G, B , W));
  }
  //FastLED.show();
  LED.show();
}
