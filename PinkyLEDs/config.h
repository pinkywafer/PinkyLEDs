#define DEVICE_NAME "PinkyLED1" //Unique name for this device
#define USE_DISCOVERY // remove if you do not want to use auto-discovery
#define USE_WHITE_BALANCE_FOR_SPEED //remove if you dp not want to use Home Assistant's built in white balance slider for animation speed

#define wifi_ssid "yourSSID" // your WIFI SSID
#define wifi_password "yourWiFiPassword" // your WIFI Password

#define mqtt_server "IP_Address" // your MQTT server address or IP.
#define mqtt_user "MQTT_USER" //enter your MQTT username
#define mqtt_password "MQTTPASSWORD" //enter your password
#define mqtt_group_topic "cmnd/PinkyLEDs" // you can use this MQTT topic as a group if you want to control multiple controllers with a single mqtt command

#define DATA_PIN    D5 // the pin the LEDs are connected to. on the NodeMCU 1.0, FastLED will default to the D5 pin after throwing an error during compiling.
#define LED_TYPE    WS2812 // your LED type WS2812 / WS2811
#define COLOR_ORDER GRB // your color order: RGB for WS2811 | GRB for WS2812
#define NUM_LEDS    150 //number of LEDs in your strip

#define POWER_BUTTON_PIN D6  // pin the Power button is connected to.  If not using the button, leave it as it is
#define COLOR_BUTTON_PIN D7 // pin the Color button is connected to.  If not using the button, leave it as it is
#define EFFECT_BUTTON_PIN 3 // pin the Effects button is connected to.  If not using the button, leave it as it is (3 is RX pin on D1 mini)

#define OTApassword "OTApassword" //the password you will need to enter to upload remotely via the ArduinoIDE

#define CHANNEL_START 1  // number of first channel for E1.31
