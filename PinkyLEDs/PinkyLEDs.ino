#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define FASTLED_ALLOW_INTERRUPTS 0
// #define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "config.h"
#ifdef ENABLE_E131
  #include <ESPAsyncE131.h>
#endif

#define VERSION "0.5.1"

int OTAport = 8266;

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
                        "Ripple", "Twinkle"};

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

#ifdef USE_DISCOVERY
  #define DISCOVERY_TOPIC "homeassistant/light/" DEVICE_NAME "/config"
  #define DISCOVERY_BASE "{ \"unique_id\": \"PinkyLED_" DEVICE_NAME "\", \"device\":{\"identifiers\":\"" DEVICE_NAME \
        "\", \"model\": \"generic\", \"manufacturer\": \"Pinkywafer\", \"name\": \"" DEVICE_NAME "\", \"sw_version\": \"" VERSION \
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
  #define DISCOVERY_PAYLOAD DISCOVERY_BASE DISCOVERY_E131 "] }" 

#endif
#ifdef ENABLE_E131
  #if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
    #warning "Requires FastLED 3.1 or later; check github for latest code."
  #endif
#endif

char setRed = 0;
char setGreen = 0;
char setBlue = 150;
String setPower = "OFF";
String setEffect = "Solid";
String setBrightness = "150";
int brightness = 150;
String setAnimationSpeed;
int animationspeed = 240;
String setColorTemp;
unsigned long flashTime = 0;
CRGB leds[NUM_LEDS];
bool startupMQTTconnect = true;

/****************FOR CANDY CANE-like desings***************/
CRGBPalette16 currentPalettestriped; //for Candy Cane
CRGBPalette16 hailPalettestriped; //for Hail
CRGBPalette16 ThxPalettestriped; //for Thanksgiving
CRGBPalette16 HalloweenPalettestriped; //for Halloween
CRGBPalette16 HJPalettestriped; //for Holly Jolly
CRGBPalette16 IndPalettestriped; //for Independence
CRGBPalette16 gPal; //for fire

/****************FOR NOISE - I'm using this one for Easter***************/
static uint16_t dist;         // A random number for our noise generator.
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.
CRGBPalette16 targetPalette(OceanColors_p);
CRGBPalette16 currentPalette(CRGB::Black);

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
char message_buff[100];


WiFiClient espClient; //this needs to be unique for each controller
PubSubClient client(espClient); //this needs to be unique for each controller
#ifdef ENABLE_E131
  ESPAsyncE131 e131(1);
#endif

void setup() {
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(POWER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(EFFECT_BUTTON_PIN, INPUT_PULLUP);
  #ifdef DEBUG 
    Serial.println("GPIO Setup complete");
  #endif
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.
  FastLED.setBrightness(brightness);
  #ifdef DEBUG 
    Serial.println("FastLED initialised"); 
  #endif
  setupStripedPalette( CRGB::Red, CRGB::Red, CRGB::White, CRGB::White); //for CANDY CANE
  setupThxPalette( CRGB::OrangeRed, CRGB::Olive, CRGB::Maroon, CRGB::Maroon); //for Thanksgiving
  setupHailPalette( CRGB::Blue, CRGB::Blue, CRGB::Yellow, CRGB::Yellow); //for HAIL
  setupHalloweenPalette( CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Indigo, CRGB::Indigo); //for Halloween
  setupHJPalette( CRGB::Red, CRGB::Red, CRGB::Green, CRGB::Green); //for Holly Jolly
  setupIndPalette( CRGB::FireBrick, CRGB::Cornsilk, CRGB::MediumBlue, CRGB::MediumBlue); //for Independence

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
  #endif
  client.setServer(mqtt_server, 1883); //CHANGE PORT HERE IF NEEDED
  client.setCallback(callback);
  #ifdef DEBUG 
    Serial.println("MQTT Initialised"); 
  #endif
  ArduinoOTA.setPort(OTAport);
  ArduinoOTA.setHostname(DEVICE_NAME);
  ArduinoOTA.setPassword((const char *)OTApassword);
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
    e131.begin(E131_UNICAST);
  #endif
}


void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

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
    }

    if (root.containsKey("effect")) {
      const char* newEffect = root["effect"];
      setEffect = newEffect;
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
      if ((setEffect == "Twinkle")||(setEffect == "Lightning")) {
        twinklecounter = 0;
        #ifdef DEBUG
          Serial.println("Twinkle Counter reset"); 
        #endif
      }
    }

    if (root.containsKey(SPEEDTOPIC)) {
      animationspeed = root[SPEEDTOPIC];
      #ifdef DEBUG 
        Serial.print("Speed Set: "); 
        Serial.println(animationspeed); 
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
  root[SPEEDTOPIC] = animationspeed;
  if (flashTime > 0){
    root["flash"] = flashTime / 1000;
  }
  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));
  #ifdef DEBUG 
    Serial.println("Done");
  #endif
  client.publish(mqttstate, buffer, true);
  #ifdef DEBUG 
    Serial.println("State Sent"); 
  #endif
}

void loop() {

  if (!client.connected()) {
    Serial.println("MQTT Disconnected...");
    reconnect();
    Serial.println("MQTT Connection attempt complete");
  } else {
    client.loop();
  }
  ArduinoOTA.handle();
  handlePowerButton();
  handleColorButton();
  handleEffectButton();
  #ifdef ENABLE_E131
  if (setEffect == "E131" && setPower == "ON") {
    digitalWrite(BUILTIN_LED, LOW);
    if (!e131.isEmpty()) {
      e131_packet_t packet;
      e131.pull(&packet);     // Pull packet from ring buffer
      for (int i = 0; i < NUM_LEDS; i++) {
        int j = i * 3 + (CHANNEL_START);
        leds[i].setRGB(packet.property_values[j], packet.property_values[j + 1], packet.property_values[j + 2]);
      }
      FastLED.setBrightness(255);
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
      //setEffect = "Solid";
      digitalWrite(BUILTIN_LED, HIGH);
      for ( int i = 0; i < NUM_LEDS; i++) {
        leds[i].fadeToBlackBy( 8 );   //FADE OFF LEDS
      }
    } else {
      digitalWrite(BUILTIN_LED, LOW);
      static unsigned int flashDelay = 0;
      if (flashTime > 0) {
        if(millis()  - flashDelay >= flashTime) {
          flashDelay = millis();
          flashOff = !flashOff;
        }
      } else {
        flashOff = false;
      }

      if (setEffect == "Christmas") {                                  // colored stripes pulsing in Shades of GREEN and RED
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = bhw2_xmas_gp;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "St Patty") {                                  // colored stripes pulsing in Shades of GREEN
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = bhw2_greenman_gp;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "Valentine") {                                  // colored stripes pulsing in Shades of PINK and RED
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = bhw2_redrosey_gp;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "Turkey Day") {                                  // colored stripes pulsing in Shades of Brown and ORANGE
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = bhw2_thanks_gp;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "Thanksgiving") {                                  // colored stripes pulsing in Shades of Red and ORANGE and Green
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* higher = faster motion */

        fill_palette( leds, NUM_LEDS,
                      startIndex, 16, /* higher = narrower stripes */
                      ThxPalettestriped, 255, LINEARBLEND);
      }

      if (setEffect == "USA") {                                  // colored stripes pulsing in Shades of Red White & Blue
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = bhw3_41_gp;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "Independence") {                        // colored stripes of Red White & Blue
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* higher = faster motion */

        fill_palette( leds, NUM_LEDS,
                      startIndex, 16, /* higher = narrower stripes */
                      IndPalettestriped, 255, LINEARBLEND);
      }


      if (setEffect == "Halloween") {                                  // colored stripes pulsing in Shades of Purple and Orange
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = Orange_to_Purple_gp;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "Go Blue") {                                  // colored stripes pulsing in Shades of Maize and Blue
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = Pills_3_gp;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "Hail") {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* higher = faster motion */

        fill_palette( leds, NUM_LEDS,
                      startIndex, 16, /* higher = narrower stripes */
                      hailPalettestriped, 255, LINEARBLEND);
      }

      if (setEffect == "Touchdown") {                 //Maize and Blue with POLICE ALL animation
        idex++;
        if (idex >= NUM_LEDS) {
          idex = 0;
        }
        int idexY = idex;
        int idexB = antipodal_index(idexY);
        int thathue = (thishuehail + 96) % 255;
        leds[idexY] = CHSV(thishuehail, thissat, 255);
        leds[idexB] = CHSV(thathue, thissat, 255);
      }

      if (setEffect == "Punkin") {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* higher = faster motion */

        fill_palette( leds, NUM_LEDS,
                      startIndex, 16, /* higher = narrower stripes */
                      HalloweenPalettestriped, 255, LINEARBLEND);
      }

      if (setEffect == "Lovey Day") {                 //Valentine's Day colors (TWO COLOR SOLID)
        idex++;
        if (idex >= NUM_LEDS) {
          idex = 0;
        }
        int idexR = idex;
        int idexB = antipodal_index(idexR);
        int thathue = (thishueLovey + 244) % 255;
        leds[idexR] = CHSV(thishueLovey, thissat, 255);
        leds[idexB] = CHSV(thathue, thissat, 255);
      }

      if (setEffect == "Holly Jolly") {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* higher = faster motion */

        fill_palette( leds, NUM_LEDS,
                      startIndex, 16, /* higher = narrower stripes */
                      HJPalettestriped, 255, LINEARBLEND);
      }

      if (setEffect == "Sinelon") {
        fadeToBlackBy( leds, NUM_LEDS, 20);
        int pos = beatsin16(13, 0, NUM_LEDS - 1);
        leds[pos] += CRGB(Rcolor, Gcolor, Bcolor);
      }

      if (setEffect == "Juggle" ) {                           // eight colored dots, weaving in and out of sync with each other
        fadeToBlackBy( leds, NUM_LEDS, 20);
        byte dothue = 0;
        for ( int i = 0; i < 8; i++) {
          leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CRGB(Rcolor, Gcolor, Bcolor);
          dothue += 32;
        }
      }

      if (setEffect == "Confetti" ) {                       // random colored speckles that blink in and fade smoothly
        fadeToBlackBy( leds, NUM_LEDS, 10);
        int pos = random16(NUM_LEDS);
        leds[pos] += CRGB(Rcolor + random8(64), Gcolor, Bcolor);
      }

      if (setEffect == "Rainbow") {
        // FastLED's built-in rainbow generator
        static uint8_t starthue = 0;    thishue++;
        fill_rainbow(leds, NUM_LEDS, thishue, deltahue);
      }

      if (setEffect == "Glitter Rainbow") {               // FastLED's built-in rainbow generator with Glitter
        static uint8_t starthue = 0;
        thishue++;
        fill_rainbow(leds, NUM_LEDS, thishue, deltahue);
        addGlitter(80);
      }

      if (setEffect == "Glitter") {
        fadeToBlackBy( leds, NUM_LEDS, 20);
        addGlitterColor(80, Rcolor, Gcolor, Bcolor);
      }

      if (setEffect == "BPM") {                                  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { //9948
          leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
      }

      if (setEffect == "Solid" & setPower == "ON" ) {          //Fill entire strand with solid color
        fill_solid(leds, NUM_LEDS, CRGB(Rcolor, Gcolor, Bcolor));
      }



      if (setEffect == "Twinkle") {
        twinklecounter = twinklecounter + 1;
        if (twinklecounter < 2) {                               //Resets strip if previous animation was running
          FastLED.clear();
          FastLED.show();
        }
        const CRGB lightcolor(8, 7, 1);
        for ( int i = 0; i < NUM_LEDS; i++) {
          if ( !leds[i]) continue; // skip black pixels
          if ( leds[i].r & 1) { // is red odd?
            leds[i] -= lightcolor; // darken if red is odd
          } else {
            leds[i] += lightcolor; // brighten if red is even
          }
        }
        if ( random8() < DENSITY) {
          int j = random16(NUM_LEDS);
          if ( !leds[j] ) leds[j] = lightcolor;
        }
      }

      if (setEffect == "Dots") {
        uint8_t inner = beatsin8(bpm, NUM_LEDS / 4, NUM_LEDS / 4 * 3);
        uint8_t outer = beatsin8(bpm, 0, NUM_LEDS - 1);
        uint8_t middle = beatsin8(bpm, NUM_LEDS / 3, NUM_LEDS / 3 * 2);
        leds[middle] = CRGB::Purple;
        leds[inner] = CRGB::Blue;
        leds[outer] = CRGB::Aqua;
        nscale8(leds, NUM_LEDS, fadeval);
      }

      if (setEffect == "Lightning") {
        twinklecounter = twinklecounter + 1;                     //Resets strip if previous animation was running
        Serial.println(twinklecounter);
        if (twinklecounter < 2) {
          FastLED.clear();
          FastLED.show();
        }
        
        ledstart = random16(NUM_LEDS);           // Determine starting location of flash
        ledlen = random16(NUM_LEDS - ledstart);  // Determine length of flash (not to go beyond NUM_LEDS-1)
        static unsigned int lightningFlashTime = 0;
        static unsigned int lightningFlashDelay = 0;
        if (millis() - lightningFlashTime >= lightningFlashDelay){
          for (int lightningFlashCounter = 0; lightningFlashCounter < random8(3, flashes); lightningFlashCounter++) {
            if (lightningFlashCounter == 0) dimmer = 5;    // the brightness of the leader is scaled down by a factor of 5
            else dimmer = random8(1, 3);          // return strokes are brighter than the leader
            fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 255 / dimmer));
            FastLED.show();                       // Show a section of LED's
            delay(random8(4, 10));                // each flash only lasts 4-10 milliseconds
            fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 0)); // Clear the section of LED's
            FastLED.show();
            if (lightningFlashCounter == 0) delay (150);   // longer delay until next flash after the leader
            delay(50 + random8(100));             // shorter delay between strokes
          }
          lightningFlashDelay = random8(frequency) * 100;        // delay between strikes
          lightningFlashTime = millis();
        }
      }

      if (setEffect == "Police One") {                    //POLICE LIGHTS (TWO COLOR SINGLE LED)
        idex++;
        if (idex >= NUM_LEDS) {
          idex = 0;
        }
        int idexR = idex;
        int idexB = antipodal_index(idexR);
        int thathue = (thishuepolice + 160) % 255;
        for (int i = 0; i < NUM_LEDS; i++ ) {
          if (i == idexR) {
            leds[i] = CHSV(thishuepolice, thissat, 255);
          }
          else if (i == idexB) {
            leds[i] = CHSV(thathue, thissat, 255);
          }
          else {
            leds[i] = CHSV(0, 0, 0);
          }
        }

      }

      if (setEffect == "Police All") {                 //POLICE LIGHTS (TWO COLOR SOLID)
        idex++;
        if (idex >= NUM_LEDS) {
          idex = 0;
        }
        int idexR = idex;
        int idexB = antipodal_index(idexR);
        int thathue = (thishuepolice + 160) % 255;
        leds[idexR] = CHSV(thishuepolice, thissat, 255);
        leds[idexB] = CHSV(thathue, thissat, 255);
      }


      if (setEffect == "Candy Cane") {
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* higher = faster motion */

        fill_palette( leds, NUM_LEDS,
                      startIndex, 16, /* higher = narrower stripes */
                      currentPalettestriped, 255, LINEARBLEND);
      }

      if (setEffect == "Cyclon Rainbow") {
        static uint8_t cyclonHue = 0;
        static bool forwards = false;
        for(int a = 0; a < NUM_LEDS; a++) {
          leds[a].nscale8(250);
        }
        static int cyclonPos = 0;
        if (forwards) {
          if (cyclonPos < NUM_LEDS - 1){
            leds[cyclonPos] = CHSV(cyclonHue++, 255,255);
            cyclonPos++;
          } else {
            forwards = false;
          }
        } else {
          if (cyclonPos >= 0) {
            leds[cyclonPos] = CHSV(cyclonHue++, 255,255);
            cyclonPos--;
          } else {
            forwards = true;
          }
        }
      }
      
      if (setEffect == "Fire") {
        Fire2012WithPalette();
      }
      random16_add_entropy( random8());

      EVERY_N_MILLISECONDS(10) {
        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // FOR NOISE ANIMATION
        {
          gHue++;
        }

        if (setEffect == "Easter") {
          setPower = "ON";
          for (int i = 0; i < NUM_LEDS; i++) {                                     // Just ONE loop to fill up the LED array as all of the pixels change.
            uint8_t index = inoise8(i * scale, dist + i * scale) % 255;            // Get a value from the noise function. I'm using both x and y axis.
            leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
          }
          dist += beatsin8(10, 1, 4);                                              // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
        }

        if (setEffect == "Ripple") {
          for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(bgcol++, 255, 15);  // Rotate background colour.
          switch (step) {
            case -1:                                                          // Initialize ripple variables.
              center = random(NUM_LEDS);
              colour = random8();
              step = 0;
              break;
            case 0:
              leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
              step ++;
              break;
            case maxsteps:                                                    // At the end of the ripples.
              step = -1;
              break;
            default:                                                             // Middle of the ripples.
              leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);   // Simple wrap from Marc Miller
              leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);
              step ++;                                                         // Next step.
              break;
          }
        }
      }

      EVERY_N_SECONDS(5) {
        targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
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
      if (animationspeed > 0 && animationspeed < 255) {  //Sets animation speed based on receieved value
        FastLED.delay((255 - (animationspeed - 1)) * 6);
      } else if (animationspeed == 0) {
        FastLED.delay(1600);
      }
    }
  }
}

void setupStripedPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  currentPalettestriped = CRGBPalette16(
                            A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                          );
}

void setupHailPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  hailPalettestriped = CRGBPalette16(
                         A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                       );
}

void setupHJPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  HJPalettestriped = CRGBPalette16(
                       A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                     );
}

void setupIndPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  IndPalettestriped = CRGBPalette16(
                        A, A, A, A, A, AB, AB, AB, AB, AB, B, B, B, B, B, B
                      );
}

void setupThxPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  ThxPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, AB, AB, AB, B, B, B, B, B, B
                      );
}

void setupHalloweenPalette( CRGB A, CRGB AB, CRGB B, CRGB BA)
{
  HalloweenPalettestriped = CRGBPalette16(
                              A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                            );
}

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


void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void addGlitterColor( fract8 chanceOfGlitter, int Rcolor, int Gcolor, int Bcolor)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB(Rcolor, Gcolor, Bcolor);
  }
}


void reconnect() {
  static unsigned long mqttReconnectMillis = 0;
  if (millis() - mqttReconnectMillis >= 5000) {
    mqttReconnectMillis = millis();
    Serial.print("Attempting MQTT connection...");
    if (client.connect(DEVICE_NAME, mqtt_user, mqtt_password, LWTTOPIC, 0, true, "Offline")) { //)) {
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
      client.subscribe(mqtt_group_topic);
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
