#define DEVICE_NAME "PinkyLED1" // Unique name for this device
#define USE_DISCOVERY // remove if you do not want to use auto-discovery
#define USE_WHITE_BALANCE_FOR_SPEED // remove if you do not want to use Home Assistant's built in white balance slider for animation speed
#define ENABLE_E131 // remove if you do not want to use E1.31

#define wifi_ssid "yourSSID" // your WIFI SSID
#define wifi_password "yourWiFiPassword" // your WIFI Password

#define mqtt_server "IP_Address" // your MQTT broker address or IP.
#define mqtt_port 1883 // your MQTT port (usually 1883 for unencrypted connection)
#define mqtt_user "MQTT_USER" //enter your MQTT username
#define mqtt_password "MQTTPASSWORD" //enter your MQTT password
#define mqtt_group_topic "cmnd/PinkyLEDs" // MQTT group topic to allow controlling multiple controllers with a single mqtt command

#define LED_TYPE    WS2812 // your LED type WS2812 / WS2811
#define COLOR_ORDER GRB // your color order (normally: RGB for WS2811 | GRB for WS2812)
#define NUM_LEDS    150 // number of LEDs in your strip
#define DATA_PIN          23 // Pin the LEDs are connected to
/*
Pin configuration:
- POWER_BUTTON_PIN              : pin the Power button (or rotary encoder push) is connected to.  (Remove if not using the button)
- COLOR_BUTTON_PIN              : pin the Color button (or rotary encoder push) is connected to.  (Remove if not using the button)
- EFFECT_BUTTON_PIN             : pin the Effects button (or rotary encoder push) is connected to.  (Remove if not using the button)
- BRIGHTNESS_ENCODER_DT         : Pin the Rotary Encoder for brightness DT pin is connected to (Remove if not using Rotary Encoder for Brightness)
- BRIGHTNESS_ENCODER_CLK        : Pin the Rotary Encoder for brightness CLK pin is connected to (Remove if not using Rotary Encoder for Brightness)
- SPEED_ENCODER_DT              : Pin the Rotary Encoder for speed DT pin is connected to (Remove if not using Rotary Encoder for Speed)
- SPEED_ENCODER_CLK             : Pin the Rotary Encoder for speed CLK pin is connected to (Remove if not using Rotary Encoder for Speed)
Don't use the pin of the built in LED! 
(D4 on the D1 mini, D0 on the NodeMCU, D2 on DOIT ESP32 DEVKIT V1. Check your board's documentation!)

Example pin config for D1 mini and NodeMCU:

#define DATA_PIN          D5
#define POWER_BUTTON_PIN  D6
#define COLOR_BUTTON_PIN  D7
#define EFFECT_BUTTON_PIN RX

Example pin config for DOIT ESP32 DEVKIT V1:
#define DATA_PIN          23
#define POWER_BUTTON_PIN  18
#define COLOR_BUTTON_PIN  19
#define EFFECT_BUTTON_PIN 21

*/

#define POWER_BUTTON_PIN  18
#define COLOR_BUTTON_PIN  19
#define EFFECT_BUTTON_PIN 21
#define BRIGHTNESS_ENCODER_DT 25
#define BRIGHTNESS_ENCODER_CLK 26
#define SPEED_ENCODER_DT 32
#define SPEED_ENCODER_CLK 33

#define BRIGHTNESS_ENCODER_LESS_STEPS // reduces the steps needed for the brightness encoder (remove if encoder is too responsive)
#define SPEED_ENCODER_LESS_STEPS // reduces the steps needed for the speed encoder (remove if encoder is too responsive)

#define OTApassword "OTApassword" //the password you will need to enter to upload remotely via the ArduinoIDE

#define CHANNEL_START 1  // number of first channel for E1.31
