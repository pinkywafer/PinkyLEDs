#define DEVICE_NAME "PinkyLED1" // Unique name for this device
#define USE_DISCOVERY // remove if you do not want to use auto-discovery
#define USE_WHITE_BALANCE_FOR_SPEED // remove if you do not want to use Home Assistant's built in white balance slider for animation speed
#define ENABLE_E131 // remove if you do not want to use E1.31

#define wifi_ssid "yourSSID" // your WIFI SSID
#define wifi_password "yourWiFiPassword" // your WIFI Password

#define mqtt_server "IP_Address" // your MQTT broker address or IP.
#define mqtt_user "MQTT_USER" //enter your MQTT username
#define mqtt_password "MQTTPASSWORD" //enter your MQTT password
#define mqtt_group_topic "cmnd/PinkyLEDs" // MQTT group topic to allow controlling multiple controllers with a single mqtt command

#define LED_TYPE    WS2812 // your LED type WS2812 / WS2811
#define COLOR_ORDER GRB // your color order (normally: RGB for WS2811 | GRB for WS2812)
#define NUM_LEDS    150 // number of LEDs in your strip

// Different pin configurations for ESP32 and NodeMCU 1.0
#if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  #define DATA_PIN          D5  // the pin the LEDs are connected to.
  #define POWER_BUTTON_PIN  D6  // pin the Power button is connected to.  If not using the button, leave it as it is
  #define COLOR_BUTTON_PIN  D7  // pin the Color button is connected to.  If not using the button, leave it as it is
  #define EFFECT_BUTTON_PIN 3   // pin the Effects button is connected to.  If not using the button, leave it as it is
#elif defined(ESP32)
  #define DATA_PIN          23  // the pin the LEDs are connected to.
  #define POWER_BUTTON_PIN  18  // pin the Power button is connected to.  If not using the button, leave it as it is
  #define COLOR_BUTTON_PIN  19  // pin the Color button is connected to.  If not using the button, leave it as it is
  #define EFFECT_BUTTON_PIN 21  // pin the Effects button is connected to.  If not using the button, leave it as it is (3 is RX pin on D1 mini)
#endif

#define OTApassword "OTApassword" //the password you will need to enter to upload remotely via the ArduinoIDE

#define CHANNEL_START 1  // number of first channel for E1.31
