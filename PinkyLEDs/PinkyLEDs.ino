/************************************************************************************************/
/*      _____                                        |       ________  ______         ____
 *      |    \                   |                   |       |         |     \       /    \
 *      |     |   o              |                   |       |         |      \     |
 *      |     |         _____    |   /   |       |   |       |         |       |    |
 *      |____/    |   |/     \   |  /    |       |   |       |______   |       |     \____
 *      |         |   |      |   | /     |       |   |       |         |       |          \
 *      |         |   |      |   |/\     |       |   |       |         |       |           |
 *      |         |   |      |   |  \     \_____/    |       |         |      /            |
 *      |         |   |      |   |   \         /     |       |_______  |_____/      \_____/ 
 *   \________________________________________/      |________________________________________/
 */
#define VERSION "0.11.0"

String (*externalEffect[])(char,bool){
  // add your custom effects to this list
  // this also sets the order of the effects list in Home Assistant
  confettiColorEffect,
  glitterColorEffect,
  juggleColorEffect,
  sinelonColorEffect,
  christmasEffect,
  candyCaneEffect,
  hollyJollyEffect,
  valentineEffect,
  loveyDayEffect,
  stPatEffect,
  easterEffect,
  usaEffect,
  independenceEffect,
  goBlueEffect,
  hailEffect,
  touchdownEffect,
  halloweenEffect,
  punkinEffect,
  thanksgivingEffect,
  turkeyDayEffect,
  bpmEffect,
  cyclonRainbowEffect,
  dotsEffect,
  fireEffect,
  lightningEffect,
  policeAllEffect,
  policeOneEffect,
  rainbowEffect,
  glitterRainbowEffect,
  rippleEffect,
  twinkleEffect
};

#define RUN 0
#define INITIALIZE 1
#define GET_NAME 2

#include <ArduinoJson.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPmDNS.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#endif

#include <PubSubClient.h>

#ifdef ESP8266
/*
  Spending too much time in the ISR or disabling the interrupts for too long
  time could cause WDT reset.

  For info see:
  https://github.com/espressif/ESP8266_NONOS_SDK/issues/90
  and
  https://github.com/FastLED/FastLED/wiki/Interrupt-problems
*/
//#define FASTLED_ALLOW_INTERRUPTS 0
  #define FASTLED_INTERRUPT_RETRY_COUNT 1
#endif

#ifdef ARDUINO_ESP8266_NODEMCU
  #define FASTLED_ESP8266_RAW_PIN_ORDER
#endif

#include <FastLED.h>
#include <ArduinoOTA.h>
#include "config.h"
#ifdef ENABLE_E131
  #include <ESPAsyncE131.h>

  #if (UNIVERSE_START < 1) || (UNIVERSE_START > 63999)
    #error "UNIVERSE_START has to be 1 - 63999, check config.h"
  #endif
#endif

#if defined(BRIGHTNESS_ENCODER_DT) || defined(SPEED_ENCODER_DT)
  #include <RotaryEncoder.h>
#endif

#ifdef ARDUINO_ESP8266_NODEMCU
  #define HW_PLATFORM "NodeMCU"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  #define HW_PLATFORM "D1 mini"
#elif defined(ESP32)
  #define HW_PLATFORM "ESP32"
#else
  #define HW_PLATFORM "other"
#endif

#define VERSION_FULL VERSION " " HW_PLATFORM

#ifdef ESP32
  #define LED_ON HIGH
  #define LED_OFF LOW
#else
  #define LED_ON LOW
  #define LED_OFF HIGH
#endif

int OTAport = 8266;
#ifdef USE_DISCOVERY
  #define DISCOVERY_TOPIC "homeassistant/light/" DEVICE_NAME "/config"
  #define DISCOVERY_BASE "{ \"unique_id\": \"PinkyLED_" DEVICE_NAME "\", \"device\":{\"identifiers\":\"" DEVICE_NAME \
        "\", \"model\": \"PinkyLEDs\", \"manufacturer\": \"Pinkywafer\", \"name\": \"" DEVICE_NAME "\", \"sw_version\": \"" VERSION_FULL \
        "\"}, \"name\": \"" DEVICE_NAME "\", \"platform\": \"mqtt\", \"schema\": \"json\", \"state_topic\": \"" mqttstate \
        "\", \"command_topic\": \"" mqttcommand "\", \"white_value\": \"" WHITE_VALUE "\", \"optimistic\": \"false\", " \
        "\"availability_topic\": \"" LWTTOPIC "\", \"payload_available\": \"Online\", \"payload_not_available\": \"Offline\", " \
        "\"rgb\": \"true\", \"flash_time_short\": \"1\", \"flash_time_long\": \"5\", \"brightness\": \"true\", " \
        "\"effect\": \"true\", \"effect_list\": ["

#endif
const byte colorList[][3] = {{255,0,0}, {0,255,0}, {0,0,255}, {0,255,127}, {191,255,0},
                        {255,255,255}, {255,255,36}, {255,191,0}, {255,127,0}, {255,163,72},
                        {255,36,36}, {255,72,118}, {255,0,127}, {255,0,255}, {191,0,255},
                        {145,36,255}, {127,0,255}, {91,36,255}, {63,0,255}, {109,109,255},
                        {72,72,255}, {0,63,255}, {36,91,255}, {109,145,255}, {0,127,255},
                        {72,163,255}, {0,191,255}, {72,209,255}, {36,255,255}, {109,255,255},
                        {255,109,109}, {163,72,255}};

String (*builtInEffect[])(char,bool){
  #ifdef AUDIO_REACTIVE_PIN
    audioColorEffect,
    audioLevelRainbowEffect,
  #endif
  solidEffect
  #ifdef ENABLE_E131
    , E131Effect
  #endif
};

const int noOfExternalEffects = sizeof(externalEffect) / sizeof(externalEffect[0]);
const int noOfBuiltInEffects = sizeof(builtInEffect) / sizeof(builtInEffect[0]);

/***** MQTT TOPICS ********************/
#ifdef USE_WHITE_BALANCE_FOR_SPEED
  #define SPEEDTOPIC "white_value"
  #define WHITE_VALUE "true"
#else
  #define SPEEDTOPIC "speed"
  #define WHITE_VALUE "false"
#endif
#define mqttcommand "cmnd/" DEVICE_NAME 
#define mqttstate "stat/" DEVICE_NAME 
#define LWTTOPIC "LWT/" DEVICE_NAME
#ifdef ENABLE_E131
  #if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3003002)
    #warning "Requires FastLED 3.3.2 or later; check github for latest code."
  #endif
#endif

int newEffect = -1;
int oldEffect = -1;
bool powerChanged = false;
char setRed = 0;
char setGreen = 0;
char setBlue = 150;
String setPower = "OFF";
int brightness = 150;
int animationSpeed = 240;
unsigned long flashTime = 0;
CRGB leds[2][NUM_LEDS];
CRGB outputLEDS[NUM_LEDS];
bool startupMQTTconnect = true;
bool fadeDone = true;
boolean useStrip = 0;

#ifdef BRIGHTNESS_ENCODER_DT
  volatile boolean brightnessEncoderChanged = false;
  RotaryEncoder brightnessEncoder(BRIGHTNESS_ENCODER_DT, BRIGHTNESS_ENCODER_CLK);
    void ICACHE_RAM_ATTR brightnessEncoderCallback(){
      brightnessEncoder.tick();
      brightnessEncoderChanged = true;
    }
#endif

#ifdef SPEED_ENCODER_DT
  volatile boolean animationSpeedEncoderChanged = false;
  RotaryEncoder animationSpeedEncoder(SPEED_ENCODER_DT, SPEED_ENCODER_CLK);
  void ICACHE_RAM_ATTR animationSpeedEncoderCallback() {
      animationSpeedEncoder.tick();
      animationSpeedEncoderChanged = true;
  }
#endif

WiFiClient espClient; 
PubSubClient client(espClient);
#ifdef ENABLE_E131
  uint16_t universesRequired;
  ESPAsyncE131* e131;
#endif

void setup() {
  Serial.begin(115200);
  #ifdef BUILTIN_LED
    pinMode(BUILTIN_LED, OUTPUT);
  #endif
  #ifdef POWER_BUTTON_PIN
    pinMode(POWER_BUTTON_PIN, INPUT_PULLUP);
  #endif
  #ifdef COLOR_BUTTON_PIN
    pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);
  #endif
  #ifdef EFFECT_BUTTON_PIN
    pinMode(EFFECT_BUTTON_PIN, INPUT_PULLUP);
  #endif
  #ifdef BRIGHTNESS_ENCODER_DT
    attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENCODER_DT), brightnessEncoderCallback, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENCODER_CLK), brightnessEncoderCallback, CHANGE);
    #ifdef BRIGHTNESS_ENCODER_DT
      #ifdef BRIGHTNESS_ENCODER_LESS_STEPS
        brightnessEncoder.setPosition(brightness / 15);
      #else
        brightnessEncoder.setPosition(brightness / 5);
      #endif
    #endif
  #endif
  #ifdef SPEED_ENCODER_DT
    attachInterrupt(digitalPinToInterrupt(SPEED_ENCODER_DT), animationSpeedEncoderCallback, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SPEED_ENCODER_CLK), animationSpeedEncoderCallback, CHANGE);
    #ifdef SPEED_ENCODER_DT
      #ifdef SPEED_ENCODER_LESS_STEPS
        animationSpeedEncoder.setPosition(animationSpeed / 15);
      #else
        animationSpeedEncoder.setPosition(animationSpeed / 5);
      #endif
    #endif
  #endif
  #ifdef AUDIO_REACTIVE_PIN
    pinMode(AUDIO_REACTIVE_PIN, INPUT);
    #ifdef ESP32
      analogReadResolution(10);
      analogSetWidth(10);
    #endif
  #endif
  #ifdef DEBUG 
    Serial.println("GPIO Setup complete");
  #endif
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(outputLEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.
  FastLED.setBrightness(brightness);
  #ifdef DEBUG 
    Serial.println("FastLED initialised"); 
  #endif
  // initially set to solid color (if it is defined)
  for (int i = 0;i< noOfBuiltInEffects;i++){
    if (builtInEffect[i](GET_NAME,0) == "Solid Color"){
      newEffect = i + noOfExternalEffects;
      break;
    }
  }
  #ifdef DEBUG
    Serial.println("Palettes initialised");
  #endif
  fill_solid(outputLEDS, NUM_LEDS, CRGB(255, 0, 0)); // Startup LED Lights
  FastLED.show();
  #ifdef DEBUG 
    Serial.println("Initial setup complete - LEDs on RED"); 
  #endif
  setup_wifi();
  #ifdef DEBUG 
    Serial.println("WiFi Setup complete"); 
    Serial.print("Hostname: ");
    #ifdef ESP32
      Serial.println(WiFi.getHostname());
    #else
      Serial.println(WiFi.hostname());
    #endif
  #endif
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
  #ifdef DEBUG 
    Serial.println("MQTT Initialised"); 
  #endif
  ArduinoOTA.setPort(OTAport);
  ArduinoOTA.setHostname(DEVICE_NAME);
  ArduinoOTA.setPassword((const char *)OTA_PASSWORD);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  #ifdef DEBUG 
    Serial.println("OTA setup complete"); 
  #endif
  #ifdef ENABLE_E131
    if ( (NUM_LEDS % 170) > 0 ){
      universesRequired = (NUM_LEDS / 170) + 1;
    } else {
      universesRequired = (NUM_LEDS / 170);
    }
    Serial.println(universesRequired);
    e131 = new ESPAsyncE131(universesRequired);
    e131->begin(E131_UNICAST);
    #ifdef DEBUG 
      Serial.printf("E131 buffer initialised with %u Buffers\n", universesRequired); 
    #endif
  #endif
}


void setup_wifi() {
  #ifdef ESP32
  WiFi.setSleep(false);
  #else
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  #endif

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);

  #ifdef ESP32
    WiFi.setHostname(DEVICE_NAME);
  #else
    WiFi.hostname(DEVICE_NAME);
  #endif

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while ((WiFi.status() != WL_CONNECTED) && (WiFi.status() != WL_NO_SSID_AVAIL) && (WiFi.status() != WL_CONNECT_FAILED)) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    fill_solid(outputLEDS, NUM_LEDS, CRGB(255, 191, 0));
    FastLED.show();
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(750);
  } else if (WiFi.status() == WL_NO_SSID_AVAIL) {
    Serial.println("");
    Serial.println("WiFi connection failed.  SSID not available");
  } else if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("");
    Serial.println("WiFi connection failed.  Unable to connect");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  Serial.print("MQTT message received ");
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  if (String(topic) == mqttstate){
    Serial.println("State message - Unsubscribing from state topic"); 
    client.unsubscribe(mqttstate);
  }
  message[length] = '\0';
  Serial.println(message);

  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    Serial.println("parseObject() failed");
  } else {
    #ifdef DEBUG 
      Serial.println("JSON message parsed succesfully"); 
    #endif
    if (root.containsKey("state")) {
      const char* power = root["state"];
      String spower = power;
      if (spower != setPower){
        setPower = power;
        powerChanged = true;
        fadeDone = false;
        if (setPower == "ON"){
          initializeEffect();
        }
      }
      #ifdef DEBUG 
        Serial.print("Power set: "); 
        Serial.println(setPower); 
      #endif
    }
  
    if (root.containsKey("color")) {
      setRed = root["color"]["r"];
      setGreen = root["color"]["g"];
      setBlue = root["color"]["b"];
      #ifdef DEBUG
        Serial.print("Colour Set: Red:");
        Serial.print(setRed, DEC);
        Serial.print("  Green: ");
        Serial.print(setGreen, DEC);
        Serial.print("  Blue: ");
        Serial.println(setBlue, DEC);
      #endif
    }

    if (root.containsKey("brightness")) {
      brightness = root["brightness"];
      #ifdef DEBUG 
        Serial.print("Brightness Set: "); 
        Serial.println(brightness); 
      #endif
      #ifdef BRIGHTNESS_ENCODER_DT
        #ifdef BRIGHTNESS_ENCODER_LESS_STEPS
          brightnessEncoder.setPosition(brightness / 15);
        #else
          brightnessEncoder.setPosition(brightness / 5);
        #endif
      #endif
    }

    if (root.containsKey("effect")) {
      const char* getEffect = root["effect"];
      String setEffect = getEffect;
      #ifdef DEBUG 
        Serial.print("Effect Set: ");
        Serial.println(setEffect); 
      #endif
      int callEffect = findEffect(setEffect);
      if (callEffect != newEffect) {
        oldEffect = newEffect;
        newEffect = callEffect;
        useStrip = !useStrip;
        Serial.println(useStrip);
        initializeEffect();
        fadeDone = false;
      }
    }

    if (root.containsKey(SPEEDTOPIC)) {
      animationSpeed = root[SPEEDTOPIC];
      #ifdef DEBUG 
        Serial.print("Speed Set: "); 
        Serial.println(animationSpeed); 
      #endif
      #ifdef SPEED_ENCODER_DT
        #ifdef SPEED_ENCODER_LESS_STEPS
          animationSpeedEncoder.setPosition(animationSpeed / 15);
        #else
          animationSpeedEncoder.setPosition(animationSpeed / 5);
        #endif
        
      #endif
    }

    if (root.containsKey("flash")) {
      flashTime = (int)root["flash"] * 1000;
      #ifdef DEBUG 
        Serial.print("Flash Set: "); 
        Serial.println(flashTime); 
      #endif
    }else{
      flashTime = 0;
      #ifdef DEBUG 
        Serial.println("Flash Set: Off"); 
      #endif
    }
  }
  #ifdef DEBUG 
    Serial.println("Prepare to publish state..."); 
  #endif
  publishState();
  #ifdef DEBUG 
    Serial.println("MQTT command complete"); 
  #endif
}

void initializeEffect(){
  if (newEffect < noOfExternalEffects){
    Serial.println(externalEffect[newEffect](INITIALIZE,useStrip));
  } else {
    builtInEffect[newEffect-noOfExternalEffects](INITIALIZE,useStrip);
  }
}

int findEffect(String effectName){
  for (int i = 0; i <= noOfExternalEffects+noOfBuiltInEffects;i++){
    if (i == noOfExternalEffects+noOfBuiltInEffects){
      return 0;
    } else {
      if (i < noOfExternalEffects){
        if (effectName == externalEffect[i](GET_NAME,0)){
          return i;
          break;
        }
      } else {
        if (effectName == builtInEffect[i-noOfExternalEffects](GET_NAME,0)){
          return i;
          break;
        }
      }
    }
  }
}

void publishState() {
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  #ifdef DEBUG 
    Serial.print("Initialising JSON State Message... "); 
  #endif
  root["state"] = setPower; 
  JsonObject& color = root.createNestedObject("color");
  color["r"] = setRed;
  color["g"] = setGreen;
  color["b"] = setBlue;
  root["brightness"] = brightness;
  if (newEffect < noOfExternalEffects){
    root["effect"] = externalEffect[newEffect](GET_NAME,0); 
  } else {
    root["effect"] = builtInEffect[newEffect-noOfExternalEffects](GET_NAME,0); 
  }
  root[SPEEDTOPIC] = animationSpeed;
  if (flashTime > 0){
    root["flash"] = flashTime / 1000;
  }
  uint8_t buffer[root.measureLength() + 1];
  root.printTo((char*)buffer, sizeof(buffer));
  #ifdef DEBUG 
    Serial.println("Done");
  #endif
  client.beginPublish(mqttstate,sizeof(buffer)-1,true);
  client.write(buffer,sizeof(buffer)-1);
  client.endPublish();
  #ifdef DEBUG 
    Serial.println("State Sent"); 
  #endif
}

void loop() {

  if (!client.connected()) {
    reconnect();
  } else {
    client.loop();
  }
  ArduinoOTA.handle();
  #ifdef POWER_BUTTON_PIN
    handlePowerButton();
  #endif
  #ifdef COLOR_BUTTON_PIN
    handleColorButton();
  #endif
  #ifdef EFFECT_BUTTON_PIN
    handleEffectButton();
  #endif
  #ifdef BRIGHTNESS_ENCODER_DT
    if (brightnessEncoderChanged) {
      brightnessFromEncoder();
    }
  #endif
  #ifdef SPEED_ENCODER_DT
    if (animationSpeedEncoderChanged) {
      animationSpeedFromEncoder();
    }
  #endif
  
  static bool flashOff = false;
  static String effectParams = "";  
  static int fadeVal = 255;
  static unsigned int flashDelay = 0;
  if (flashTime > 0) {
    if(millis()  - flashDelay >= flashTime) {
      flashDelay = millis();
      flashOff = !flashOff;
    }
  } else {
    flashOff = false;
  }
  if (setPower == "OFF") {
    powerOff(useStrip);
  } else {
    #ifdef BUILTIN_LED
      digitalWrite(BUILTIN_LED, LED_ON);
    #endif
    if (newEffect < noOfExternalEffects){
      effectParams = externalEffect[newEffect](RUN,useStrip);
    } else {
      effectParams = builtInEffect[newEffect-noOfExternalEffects](RUN, useStrip);
    }
  }
  
  if (!fadeDone){
    int fadeEffect = oldEffect;
    if (powerChanged){
      if (setPower == "ON"){
        fadeEffect = -1;
      } else {
        fadeEffect = newEffect;
      }
    } else {
      fadeEffect = oldEffect;
    }
    if (fadeEffect == -1){
      powerOff(!useStrip);
    } else if (fadeEffect < noOfExternalEffects){
      externalEffect[fadeEffect](RUN, !useStrip);
    } else {
      builtInEffect[fadeEffect-noOfExternalEffects](RUN,!useStrip);
    }
    static uint8_t fadeVal = 255;
    static int fadeMillis = millis();
    if (millis() > fadeMillis + 30){
      fadeVal -=5;
      fadeMillis = millis();
    }
    for (int i = 0; i<NUM_LEDS;i++){
      outputLEDS[i] = blend(leds[useStrip][i], leds[!useStrip][i], fadeVal);
    }
    if (fadeVal == 0){
      fill_solid( leds[!useStrip], NUM_LEDS, CRGB::Black);
      fadeDone = true;
      powerChanged = false;
      fadeVal = 255;
    }
  } else {
      memcpy(outputLEDS, leds[useStrip], sizeof(leds[useStrip][0])*NUM_LEDS);
  }
  
  if (flashOff){
    FastLED.clear(true);
  }
  if (effectParams.indexOf("BR") < 0){
    FastLED.setBrightness(brightness);  
  }
  FastLED.show();
}

void reconnect() {
  static unsigned long mqttReconnectMillis = 0;
  if (millis() - mqttReconnectMillis >= 5000) {
    mqttReconnectMillis = millis();
    Serial.print("Attempting MQTT connection...");
    if (client.connect(DEVICE_NAME, MQTT_USERNAME, MQTT_PASSWORD, LWTTOPIC, 0, true, "Offline")) {
      Serial.println("connected");
      client.publish(LWTTOPIC, "Online", true);
      #ifdef USE_DISCOVERY
        String payload = DISCOVERY_BASE;
        if (noOfExternalEffects > 0) {
          for (int i = 0; i < noOfExternalEffects; i++){
            payload += "\"" + externalEffect[i](GET_NAME,0) + "\"";
            if (i + 1 < noOfExternalEffects){
              payload += ", ";
            }
          }
          if (noOfExternalEffects > 0){
            payload += ", ";
          }
        }
        if (noOfBuiltInEffects > 0) {
          for (int i = 0; i < noOfBuiltInEffects; i++){
            payload += "\"" + builtInEffect[i](GET_NAME,0) + "\"";
            if (i + 1 < noOfBuiltInEffects){
              payload += ", ";
            }
          }
        }
        payload += "] }";
        byte b[payload.length()+1];
        payload.getBytes(b, payload.length()+1);
        client.beginPublish(DISCOVERY_TOPIC,sizeof(b)-1,true);//){;
        client.write(b,sizeof(b)-1);
        client.endPublish();
        #ifdef DEBUG
          for (int i = 0; i<payload.length()+1;i++){
          Serial.print((char) b[i]); }
          Serial.println();
          Serial.println("Discovery message sent"); 
        #endif
      #endif
      client.subscribe(mqttcommand);
      #ifdef MQTT_GROUP_TOPIC
        client.subscribe(MQTT_GROUP_TOPIC);
      #endif
      #ifdef DEBUG 
        Serial.println("Subscribed to MQTT topics");
      #endif
      if (startupMQTTconnect) {
        client.subscribe(mqttstate);
        startupMQTTconnect = false;
        #ifdef DEBUG
          Serial.println("Subscribed to MQTT State topic"); 
        #endif
        fill_solid(outputLEDS, NUM_LEDS, CRGB(0, 255, 0));
        FastLED.show();
        delay(500);
        FastLED.clear (true); //Turns off startup LEDs after connection is made
        FastLED.show();
        delay(250);
      } else {
        publishState();
        #ifdef DEBUG 
          Serial.println("State published"); 
        #endif
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      if (startupMQTTconnect) {
        startupMQTTconnect = false;
        FastLED.clear(true);
        FastLED.show();
        delay(250);
      } 
    }
  }
}
#ifdef POWER_BUTTON_PIN
  void handlePowerButton() {
    static unsigned int powerDebounce = 0;
    static int lastPowerButtonState = HIGH;
    if (millis() - powerDebounce >= 50) { //avoid delay for debounce
      powerDebounce = millis();
      int powerButtonState = digitalRead(POWER_BUTTON_PIN);
      if (powerButtonState != lastPowerButtonState) { // button state has changed
        if (powerButtonState == LOW) { // button is pressed
          if (setPower == "ON") { //if on, turn off
            setPower = "OFF";
          } else { //if off turn on
            setPower = "ON";
            initializeEffect();
          }
          powerChanged = true;
          fadeDone = false;
          publishState();
          Serial.println("Button press - Set Power: " + setPower);
        }
      }
      lastPowerButtonState = powerButtonState;
    }
  }
#endif
#ifdef COLOR_BUTTON_PIN
  void handleColorButton() {
    static int nextColor = 0;
    static unsigned int colorDebounce = 0;
    static int lastColorButtonState = HIGH;
    if (millis() - colorDebounce >= 50) { //avoid delay for debounce
      colorDebounce = millis();
      int colorButtonState = digitalRead(COLOR_BUTTON_PIN);
      if (colorButtonState != lastColorButtonState) { // button state has changed
        if (colorButtonState == LOW) { // button is pressed
          setRed = colorList[nextColor][0];
          setGreen = colorList[nextColor][1];
          setBlue = colorList[nextColor][2];
          if (nextColor + 1 >= (sizeof(colorList) / 3)){
            nextColor = 0;
          } else {
            nextColor++;
          }
          publishState();
        }
      }
      lastColorButtonState = colorButtonState;
    }
  }
#endif
#ifdef EFFECT_BUTTON_PIN
  void handleEffectButton() {
    static unsigned int effectDebounce = 0;
    static int lastEffectButtonState = HIGH;
    if (millis() - effectDebounce >= 50) { //avoid delay for debounce
      effectDebounce = millis();
      int effectButtonState = digitalRead(EFFECT_BUTTON_PIN);
      if (effectButtonState != lastEffectButtonState) { // button state has changed
        if (effectButtonState == LOW) { // button is pressed
            oldEffect = newEffect;
            if (newEffect + 1 < noOfExternalEffects + noOfBuiltInEffects){
              newEffect++;
            } else {
              newEffect = 0;
            }
            if (newEffect>=noOfExternalEffects){
              if (builtInEffect[newEffect-noOfExternalEffects](GET_NAME,0) == "E131"){
                if (newEffect + 1 >= noOfExternalEffects + noOfBuiltInEffects){
                  newEffect = 0;
                }else{
                  newEffect++;
                }
              }
            }
          useStrip = !useStrip;
          initializeEffect();
          fadeDone = false;
          publishState();
        }
      }
      lastEffectButtonState = effectButtonState;
    }
  }
#endif
#ifdef BRIGHTNESS_ENCODER_DT
  void brightnessFromEncoder(){
    #ifdef BRIGHTNESS_ENCODER_LESS_STEPS
      int encoderSpeed = 15;
    #else
      int encoderSpeed = 5;
    #endif
    int newPos = brightnessEncoder.getPosition() * encoderSpeed;
    if (newPos != brightness) {
      if (newPos < 0) {
          brightnessEncoder.setPosition(0);
          newPos = 0;
      } else if (newPos > 255) {
          brightnessEncoder.setPosition(255 / encoderSpeed);
          newPos = 255;
      }
      brightness = newPos;
      publishState();
    } 
  }
#endif

#ifdef SPEED_ENCODER_DT
  void animationSpeedFromEncoder(){
    #ifdef SPEED_ENCODER_LESS_STEPS
      int encoderSpeed = 15;
    #else
      int encoderSpeed = 5;
    #endif
    int newPos = animationSpeedEncoder.getPosition() * encoderSpeed;
    if (newPos != animationSpeed) {
      if (newPos < 0) {
          animationSpeedEncoder.setPosition(0);
          newPos = 0;
      } else if (newPos > 255) {
          animationSpeedEncoder.setPosition(255 / encoderSpeed);
          newPos = 255;
      }
      animationSpeed = newPos;
      publishState();
    } 
  }
#endif

String solidEffect(char mode, bool strip){
  switch (mode){
    case RUN: 
      {
        fill_solid(leds[strip], NUM_LEDS, CRGB(setRed, setGreen, setBlue));
      }
      return "";
      break;
    default:
      return "Solid Color";
  }
}

#ifdef ENABLE_E131
  String E131Effect(char mode, bool strip){
    static boolean clearStrip = false;
    switch (mode){
      case RUN:
        {
          #ifdef BUILTIN_LED
            digitalWrite(BUILTIN_LED, LED_ON);
          #endif
          if (clearStrip){
            fill_solid(leds[strip], NUM_LEDS, CRGB(0, 0, 0));
            clearStrip = false;
            FastLED.setBrightness(255);
          }
          if (!e131->isEmpty()) {
            e131_packet_t packet;
            e131->pull(&packet);     // Pull packet from ring buffer
            
            uint16_t universe = htons(packet.universe);
            uint16_t universeLast = universe + universesRequired - 1;
            uint16_t maxChannels = htons(packet.property_value_count) - 1;
      
            if ( universe >= UNIVERSE_START ) 
            {
              // Calculate LED range to update
              uint16_t firstLed = ((universe - UNIVERSE_START) * 170);
              uint16_t lastLed  = firstLed + (maxChannels / 3);  // -1
      
              #ifdef DEBUG
                Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / FirstLed: %3u/ LastLed: %3u / CH1: %3u / CH2: %3u / CH3: %3u\n",
                          universe,                               // The Universe for this packet
                          maxChannels,                            // Start code is ignored, we're interested in dimmer data
                          e131->stats.num_packets,                 // Packet counter
                          e131->stats.packet_errors,               // Packet error counter
                          firstLed,                               // First LED to update
                          lastLed-1,                              // Last LED to update
                          packet.property_values[1],              // Dimmer data for Channel 1
                          packet.property_values[2],              // Dimmer data for Channel 2
                          packet.property_values[3]);             // Dimmer data for Channel 3
              #endif
      
              int j = 1;
              for (int i = firstLed; i < min(lastLed,(uint16_t)NUM_LEDS); i++)
              {
                // Calculate channel
                leds[strip][i].setRGB(packet.property_values[j], packet.property_values[j + 1], packet.property_values[j + 2]);
                j += 3;
              }
      
              FastLED.setBrightness(255);
            }
          } 
        }
        return "BR";
        break;
      case INITIALIZE:
        clearStrip = true;
        return "E131";
        break;
      case GET_NAME:
        return "E131";
    }
  }
#endif

#ifdef AUDIO_REACTIVE_PIN
  int getAudioLevel(){
    unsigned int inputMax = 0;
    unsigned int inputMin = 1024;
    #ifdef ESP32
      int average = 0;
      for (unsigned int i = 0; i<300;i++){
      average = analogRead(AUDIO_REACTIVE_PIN);
      for (int i = 0; i < 20; i++) {
        average = average + (analogRead(AUDIO_REACTIVE_PIN) - average) / 64;
      }
      inputMin = min(inputMin, (unsigned int) average);
      inputMax = max(inputMax, (unsigned int) average);
    }
    #else
      for (unsigned int i = 0; i<200;i++){
        unsigned int inputSample = analogRead(AUDIO_REACTIVE_PIN);
        inputMin = min(inputMin, inputSample);
        inputMax = max(inputMax, inputSample);
        yield();
      }
    #endif
    return inputMax-inputMin;
  }

  String audioColorEffect(char mode, bool strip){
    switch (mode){
      case RUN:
        {
          int Rcolor = setRed;
          int Gcolor = setGreen;
          int Bcolor = setBlue;
          fill_solid(leds[strip], NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
          int level = map(getAudioLevel(), AUDIO_LOW_LEVEL, AUDIO_HIGH_LEVEL, -20, 250);
          if (level < 0) level = 0;
          if (level > 250) level = 250;
          FastLED.setBrightness(level);
        }
        return "BR";
        break;
      default:
        return "Audio Color";
    }
  }

  String audioLevelRainbowEffect(char mode, bool strip){
    static int hue = 0;
    switch (mode){
      case RUN:
        {
          int level = map(getAudioLevel(), AUDIO_LOW_LEVEL, AUDIO_HIGH_LEVEL, 0, NUM_LEDS / 2);
          if (level < 0) level = 0;
          if (level > NUM_LEDS / 2) level = NUM_LEDS / 2;
          for(int a = 0; a < NUM_LEDS; a++) {
            leds[strip][a].nscale8(255-animationSpeed);
          }
          for ( int i = 0; i < level; i++) {
            leds[strip][i + (NUM_LEDS / 2)] = CHSV(hue+i, 255,255);
            leds[strip][(NUM_LEDS / 2) - i] = CHSV(hue+i, 255,255);
          }
          hue ++;
        }
        return "";
        break;
      case INITIALIZE:
        hue = 0;
        return "";
        break;
      case GET_NAME:
        return "Audio Level Rainbow";
    }
  }
#endif

void powerOff(bool strip){
  #ifdef BUILTIN_LED
      digitalWrite(BUILTIN_LED, LED_OFF);
  #endif
  fill_solid(leds[strip], NUM_LEDS, CRGB::Black);
}
