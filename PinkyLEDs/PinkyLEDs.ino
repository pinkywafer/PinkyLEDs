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
#define VERSION "0.10.1"

String (*effect[])(boolean initialize){
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
        "\", \"model\": \"generic\", \"manufacturer\": \"Pinkywafer\", \"name\": \"" DEVICE_NAME "\", \"sw_version\": \"" VERSION_FULL \
        "\"}, \"name\": \"" DEVICE_NAME "\", \"platform\": \"mqtt\", \"schema\": \"json\", \"state_topic\": \"" mqttstate \
        "\", \"command_topic\": \"" mqttcommand "\", \"white_value\": \"" WHITE_VALUE "\", \"optimistic\": \"false\", " \
        "\"availability_topic\": \"" LWTTOPIC "\", \"payload_available\": \"Online\", \"payload_not_available\": \"Offline\", " \
        "\"rgb\": \"true\", \"flash_time_short\": \"1\", \"flash_time_long\": \"5\", \"brightness\": \"true\", " \
        "\"effect\": \"true\", \"effect_list\": [\"Confetti\", \"Glitter\", \"Juggle\", \"Sinelon\", \"Solid\", " \
        "\"Christmas\", \"Candy Cane\", \"Holly Jolly\", \"Valentine\", \"Lovey Day\", \"St Patty\", \"Easter\", " \
        "\"USA\", \"Independence\", \"Go Blue\", \"Hail\", \"Touchdown\", \"Halloween\", \"Punkin\", \"Thanksgiving\", " \
        "\"Turkey Day\", \"BPM\", \"Cyclon Rainbow\", \"Dots\", \"Fire\", \"Lightning\", \"Police All\", \"Police One\", " \
        "\"Rainbow\", \"Glitter Rainbow\", \"Ripple\", \"Twinkle\""
  #ifdef ENABLE_E131
    #define DISCOVERY_E131 ",\"E131\""
  #else
    #define DISCOVERY_E131 ""
  #endif
  #ifdef AUDIO_REACTIVE_PIN
    #define DISCOVERY_REACTIVE ",\"Audio Color\", \"Audio Level Rainbow\""
    #define AUDIO_EFFECTS , "Audio Color", "Audio Level Rainbow"
  #else
    #define DISCOVERY_REACTIVE ""
    #define AUDIO_EFFECTS
  #endif
  #define DISCOVERY_PAYLOAD DISCOVERY_BASE DISCOVERY_REACTIVE DISCOVERY_E131 "] }" 

#endif
const byte colorList[][3] = {{255,0,0}, {0,255,0}, {0,0,255}, {0,255,127}, {191,255,0},
                        {255,255,255}, {255,255,36}, {255,191,0}, {255,127,0}, {255,163,72},
                        {255,36,36}, {255,72,118}, {255,0,127}, {255,0,255}, {191,0,255},
                        {145,36,255}, {127,0,255}, {91,36,255}, {63,0,255}, {109,109,255},
                        {72,72,255}, {0,63,255}, {36,91,255}, {109,145,255}, {0,127,255},
                        {72,163,255}, {0,191,255}, {72,209,255}, {36,255,255}, {109,255,255},
                        {255,109,109}, {163,72,255}};
const char effectList[][20] = {"Confetti", "Glitter", "Juggle", "Sinelon", "Solid",
                        "Christmas", "Candy Cane", "Holly Jolly", "Valentine", "Lovey Day",
                        "St Patty", "Easter", "USA", "Independence", "Go Blue",
                        "Hail", "Touchdown", "Halloween", "Punkin", "Thanksgiving",
                        "Turkey Day", "BPM", "Cyclon Rainbow", "Dots", "Fire",
                        "Lightning", "Police All", "Police One", "Rainbow", "Glitter Rainbow",
                        "Ripple", "Twinkle" AUDIO_EFFECTS};

String effectNames[sizeof(effect) / sizeof(effect[0])];
//palette for Turkey Day
DEFINE_GRADIENT_PALETTE( bhw2_thanks_gp ) {
  0,   9,  5,  1,
  48,  25,  9,  1,
  76, 137, 27,  1,
  96,  98, 42,  1,
  124, 144, 79,  1,
  153,  98, 42,  1,
  178, 137, 27,  1,
  211,  23,  9,  1,
  255,   9,  5,  1
};

//palette for Valentine
DEFINE_GRADIENT_PALETTE( bhw2_redrosey_gp ) {
  0, 103,  1, 10,
  33, 109,  1, 12,
  76, 159,  5, 48,
  119, 175, 55, 103,
  127, 175, 55, 103,
  178, 159,  5, 48,
  221, 109,  1, 12,
  255, 103,  1, 10
};

//palette for Christmas
DEFINE_GRADIENT_PALETTE( bhw2_xmas_gp ) {
  0,   0, 12,  0,
  40,   0, 55,  0,
  66,   1, 117,  2,
  77,   1, 84,  1,
  81,   0, 55,  0,
  119,   0, 12,  0,
  153,  42,  0,  0,
  181, 121,  0,  0,
  204, 255, 12,  8,
  224, 121,  0,  0,
  244,  42,  0,  0,
  255,  42,  0,  0
};

//palette for USA
DEFINE_GRADIENT_PALETTE( bhw3_41_gp ) {
  0,   0,  0, 45,
  71,   7, 12, 255,
  76,  75, 91, 255,
  76, 255, 255, 255,
  81, 255, 255, 255,
  178, 255, 255, 255,
  179, 255, 55, 45,
  196, 255,  0,  0,
  255,  42,  0,  0
};

//palette for St Patty
DEFINE_GRADIENT_PALETTE( bhw2_greenman_gp ) {
  0,   1, 22,  1,
  130,   1, 168,  2,
  255,   1, 22,  1
};

//palette for Go Blue
DEFINE_GRADIENT_PALETTE( Pills_3_gp ) {
  0,   4, 12, 122,
  127,  55, 58, 50,
  255, 192, 147, 11
};

//palette for Halloween
DEFINE_GRADIENT_PALETTE( Orange_to_Purple_gp ) {
  0, 208, 50,  1,
  127, 146, 27, 45,
  255,  97, 12, 178
};

/****************************** MQTT TOPICS (change these topics as you wish)  ***************************************/
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

boolean internalEffect = true;
int newEffect = 0;
char setRed = 0;
char setGreen = 0;
char setBlue = 150;
String setPower = "OFF";
String setEffect = "Solid";
int brightness = 150;
int animationSpeed = 240;
unsigned long flashTime = 0;
CRGB leds[NUM_LEDS];
bool startupMQTTconnect = true;

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

/****************FOR CANDY CANE-like desings***************/
CRGBPalette16 gPal; //for fire

/*  EASTER    ***************FOR NOISE - I'm using this one for Easter***************/
static uint16_t dist;         // A random number for our noise generator.
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.
//CRGBPalette16 targetPalette(OceanColors_p);
//CRGBPalette16 currentPalette(CRGB::Black);

/*****************For TWINKLE********/
#define DENSITY     80
int twinklecounter = 0;

/*********FOR RIPPLE***********/
uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.
uint8_t bgcol = 0;                                            // Background colour rotates.
int thisdelay = 20;                                           // Standard delay value.

/**************FOR RAINBOW***********/
uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;

/**************FOR DOTS**************/
uint8_t   count =   0;                                        // Count up to 255 and then reverts to 0
uint8_t fadeval = 224;                                        // Trail behind the LED's. Lower => faster fade.
uint8_t bpm = 30;

/**************FOR LIGHTNING**************/
uint8_t frequency = 50;                                       // controls the interval between strikes
uint8_t flashes = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer = 1;
uint16_t ledstart;                                            // Starting location of a flash
uint16_t ledlen;
int lightningcounter = 0;

/********FOR FUNKBOX EFFECTS**********/
int idex = 0;                //-LED INDEX (0 to NUM_LEDS-1
int TOP_INDEX = int(NUM_LEDS / 2);
int thissat = 255;           //-FX LOOPS DELAY VAR

//////////////////add thishue__ for Police All custom effects here/////////////////////////////////////////////////////////
/////////////////use hsv Hue number for one color, for second color change "thishue__ + __" in the setEffect section//////

uint8_t thishuepolice = 0;
uint8_t thishuehail = 64;
uint8_t thishueLovey = 0;
int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % NUM_LEDS;
  }
  return iN;
}

/********FIRE**********/
#define COOLING  55
#define SPARKING 120
bool gReverseDirection = false;

/********BPM**********/
uint8_t gHue = 0;

WiFiClient espClient; //this needs to be unique for each controller
PubSubClient client(espClient); //this needs to be unique for each controller
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
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.
  FastLED.setBrightness(brightness);
  #ifdef DEBUG 
    Serial.println("FastLED initialised"); 
  #endif
  //setupStripedPalette( CRGB::Red, CRGB::Red, CRGB::White, CRGB::White); //for CANDY CANE
//  setupThxPalette( CRGB::OrangeRed, CRGB::Olive, CRGB::Maroon, CRGB::Maroon); //for Thanksgiving
//  setupHailPalette( CRGB::Blue, CRGB::Blue, CRGB::Yellow, CRGB::Yellow); //for HAIL
//  setupHalloweenPalette( CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Indigo, CRGB::Indigo); //for Halloween
 // setupHJPalette( CRGB::Red, CRGB::Red, CRGB::Green, CRGB::Green); //for Holly Jolly
//  setupIndPalette( CRGB::FireBrick, CRGB::Cornsilk, CRGB::MediumBlue, CRGB::MediumBlue); //for Independence
  for (int i = 0;i< sizeof(effect) / sizeof(effect[0]);i++){
    effectNames[i] = effect[i](true);
  }
  for (int i = 0;i< sizeof(effect) / sizeof(effect[0]);i++){
    Serial.println(effectNames[i]);
  }
  gPal = HeatColors_p; //for FIRE
  #ifdef DEBUG
    Serial.println("Palettes initialised");
  #endif
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0)); //Startup LED Lights
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
  client.setServer(MQTT_BROKER, MQTT_PORT); //CHANGE PORT HERE IF NEEDED
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
    fill_solid(leds, NUM_LEDS, CRGB(255, 191, 0));
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
      setPower = power;
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
      setEffect = getEffect;
      #ifdef DEBUG 
        Serial.print("Effect Set: ");
        Serial.println(setEffect); 
      #endif
      if (setEffect == "E131") {
        FastLED.clear(true);
        #ifdef DEBUG 
          Serial.println("LEDs Cleared.  Ready for E1.31"); 
        #endif
      }
      if (setEffect == "E131" || setEffect == "Solid" || setEffect == "Audio Color" || setEffect == "Audio Level Rainbow"){
        internalEffect = true;
        Serial.println("Internal Effect Set");
      } else {
        internalEffect = false;
        ///////////////////////   lookup effect
        for (int i = 0; i< sizeof(effect) / sizeof(effect[0]);i++){
          if (setEffect == effectNames[i]){
            newEffect = i;
          }
        }
      }
      if ((setEffect == "Twinkle")||(setEffect == "Lightning")) {
        twinklecounter = 0;
        #ifdef DEBUG
          Serial.println("Twinkle Counter reset"); 
        #endif
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
  root["effect"] = setEffect; 
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
    //Serial.println("MQTT Disconnected...");
    reconnect();
    //Serial.println("MQTT Connection attempt complete");
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
  #ifdef ENABLE_E131
  if (setEffect == "E131" && setPower == "ON") {
    #ifdef BUILTIN_LED
      digitalWrite(BUILTIN_LED, LED_ON);
    #endif
    if (!e131->isEmpty()) {
      Serial.println("good");
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
          leds[i].setRGB(packet.property_values[j], packet.property_values[j + 1], packet.property_values[j + 2]);
          j += 3;
        }

        FastLED.setBrightness(255);
        FastLED.show();
      }
    }
  } else
  #endif
  #ifdef AUDIO_REACTIVE_PIN
  if ((setEffect == "Audio Color" || setEffect == "Audio Level Rainbow") && setPower == "ON") {
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
    if (setEffect == "Audio Color"){
      int Rcolor = setRed;
      int Gcolor = setGreen;
      int Bcolor = setBlue;
      fill_solid(leds, NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
      int level = constrain(map((inputMax-inputMin), AUDIO_LOW_LEVEL, AUDIO_HIGH_LEVEL, -20, 250),0,250);
      FastLED.setBrightness(level);
      FastLED.show();
    } else if (setEffect == "Audio Level Rainbow"){
      static int hue = 0;
      int level = constrain(map((inputMax-inputMin), AUDIO_LOW_LEVEL, AUDIO_HIGH_LEVEL, 0, NUM_LEDS / 2),0,NUM_LEDS / 2);
      FastLED.clear();
      for ( int i = 0; i < level; i++) {
        leds[i + (NUM_LEDS / 2)] = CHSV(hue+i, 255,255);
        leds[(NUM_LEDS / 2) - i] = CHSV(hue+i, 255,255);
      }
      hue ++;
      FastLED.setBrightness(brightness);
      FastLED.show();
    }
  } else
  #endif
  {
    int Rcolor = setRed;
    int Gcolor = setGreen;
    int Bcolor = setBlue; 
    static bool flashOff = false;
    
    if (setPower == "OFF") {
      #ifdef BUILTIN_LED
        digitalWrite(BUILTIN_LED, LED_OFF);
      #endif
      for ( int i = 0; i < NUM_LEDS; i++) {
        leds[i].fadeToBlackBy( 8 );   //FADE OFF LEDS
      }
    } else {
      #ifdef BUILTIN_LED
        digitalWrite(BUILTIN_LED, LED_ON);
      #endif
      static unsigned int flashDelay = 0;
      if (flashTime > 0) {
        if(millis()  - flashDelay >= flashTime) {
          flashDelay = millis();
          flashOff = !flashOff;
        }
      } else {
        flashOff = false;
      }

      if (!internalEffect) {
        effect[newEffect](false);
      } else if (setEffect == "Solid" ){
        fill_solid(leds, NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
      } else {
        
      }
    }
    if (flashOff){
      FastLED.clear(true);
    }else{
      FastLED.setBrightness(brightness);  //EXECUTE EFFECT COLOR
      FastLED.show();
    }
    
    if (setPower == "OFF"){
      FastLED.delay(10);
    } else {
      if (animationSpeed > 0 && animationSpeed < 255) {  //Sets animation speed based on receieved value
        FastLED.delay((255 - (animationSpeed - 1)) * 6);
      } else if (animationSpeed == 0) {
        FastLED.delay(1600);
      }
    }
  }
}

//void setupStripedPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
//{
//  currentPalettestriped = CRGBPalette16(
//                            A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
//                          );
//}

//void setupHailPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
//{
//  hailPalettestriped = CRGBPalette16(
//                         A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
//                       );
//}

//void setupHJPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
//{
//  HJPalettestriped = CRGBPalette16(
//                       A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
//                     );
//}

//void setupIndPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
//{
//  IndPalettestriped = CRGBPalette16(
//                        A, A, A, A, A, AB, AB, AB, AB, AB, B, B, B, B, B, B
//                      );
//}

//void setupThxPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
//{
//  ThxPalettestriped = CRGBPalette16(
//                        A, A, A, A, A, A, A, AB, AB, AB, B, B, B, B, B, B
//                      );
//}

//void setupHalloweenPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
//{
//  HalloweenPalettestriped = CRGBPalette16(
//                              A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
//                            );
//}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);  //for CYCLON
  }
}

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}


/*void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}*/

/*void addGlitterColor( fract8 chanceOfGlitter, int Rcolor, int Gcolor, int Bcolor)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB(Rcolor, Gcolor, Bcolor);
  }
}
*/

void reconnect() {
  static unsigned long mqttReconnectMillis = 0;
  if (millis() - mqttReconnectMillis >= 5000) {
    mqttReconnectMillis = millis();
    Serial.print("Attempting MQTT connection...");
    if (client.connect(DEVICE_NAME, MQTT_USERNAME, MQTT_PASSWORD, LWTTOPIC, 0, true, "Offline")) { //)) {
      Serial.println("connected");
      client.publish(LWTTOPIC, "Online", true);
      #ifdef USE_DISCOVERY
        byte b[] = DISCOVERY_PAYLOAD;
        client.beginPublish(DISCOVERY_TOPIC,sizeof(b)-1,true);//){;
        client.write(b,sizeof(b)-1);
        client.endPublish();
        #ifdef DEBUG 
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
        fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0));
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
          }
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
    static int nextEffect = 0;
    static unsigned int effectDebounce = 0;
    static int lastEffectButtonState = HIGH;
    if (millis() - effectDebounce >= 50) { //avoid delay for debounce
      effectDebounce = millis();
      int effectButtonState = digitalRead(EFFECT_BUTTON_PIN);
      if (effectButtonState != lastEffectButtonState) { // button state has changed
        if (effectButtonState == LOW) { // button is pressed
          setEffect = effectList[nextEffect];
          if (setEffect == "Twinkle") {
            twinklecounter = 0;
          }
          if (setEffect == "Lightning") {
            twinklecounter = 0;
          }
          if (nextEffect + 1 >= (sizeof(effectList) / 20)){
            nextEffect = 0;
          } else {
            nextEffect++;
          }
          Serial.print("Button press - Set Effect: " + String(setEffect));
          Serial.println("Next Color index: " + String(nextEffect));
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
