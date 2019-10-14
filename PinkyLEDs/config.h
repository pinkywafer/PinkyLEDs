// For more Information on this configuration file, please see the Wiki
// (https://github.com/pinkywafer/PinkyLEDs/wiki)

//    REQUIRED CONFIGURATION     
// WIFI //
#define WIFI_SSID     "yourSSID"         // your WIFI SSID
#define WIFI_PASSWORD "yourWiFiPassword" // your WIFI Password
// MQTT //
#define MQTT_BROKER   "MQTTipAddress"    // your MQTT broker address or IP.
#define MQTT_PORT     1883               // your MQTT port (usually 1883 for unencrypted connection)
#define MQTT_USERNAME "MQTTuser"         // your MQTT username
#define MQTT_PASSWORD "MQTTpassword"     // your MQTT password

//DEVICE CONFIG //
#define DEVICE_NAME   "PinkyLED1"        // Unique name for this device
#define LED_TYPE      WS2812             // your LED type WS2812 / WS2811
#define COLOR_ORDER   GRB                // your color order (normally: RGB for WS2811 | GRB for WS2812)
#define NUM_LEDS      150                // number of LEDs in your strip
#define DATA_PIN      D3                 // Pin the LEDs are connected to
#define OTA_PASSWORD  "OTA_PASSWORD"     //the password you will need to enter to upload remotely via the ArduinoIDE

//            OPTIONS              
#define USE_DISCOVERY                     // remove if you do not want to use auto-discovery
#define USE_WHITE_BALANCE_FOR_SPEED       // remove if you do not want to use Home Assistant's built in white balance slider for animation speed
#define MQTT_GROUP_TOPIC "cmnd/PinkyLEDs" // MQTT group topic set to your own choice of group or remove if not wanted
#define ENABLE_E131                       // remove if you do not want to use E1.31
#define UNIVERSE_START 1                  // first universe to listen for for E1.31 (remove if not using E1.31

#define POWER_BUTTON_PIN       D4         // The pin your power button is connected to (Remove if not using power button)
#define COLOR_BUTTON_PIN       D7         // The pin your color button is connected to (Remove if not using color button)
#define EFFECT_BUTTON_PIN      RX         // The pin your effect button is connected to (Remove if not effect power button)
#define BRIGHTNESS_ENCODER_DT  D1         // The pin your Brightness encoder DT is connected to (Remove if not using brightness encoder)
#define BRIGHTNESS_ENCODER_CLK D2         // The pin your Brightness encoder CLK is connected to (Remove if not using brightness encoder)
#define SPEED_ENCODER_DT       D5         // The pin your Speed encoder DT is connected to (Remove if not using Speed encoder)
#define SPEED_ENCODER_CLK      D6         // The pin your Speed encoder CLK is connected to (Remove if not using Speed encoder)

#define BRIGHTNESS_ENCODER_LESS_STEPS     // reduces the steps needed for the brightness encoder (remove if encoder is too responsive)
#define SPEED_ENCODER_LESS_STEPS          // reduces the steps needed for the speed encoder (remove if encoder is too responsive)

#define AUDIO_REACTIVE_PIN   A0           // The pin your analog mic is connected to (Remove if not using audio input)
#define AUDIO_HIGH_LEVEL     50           // Audio High level adjustment (Remove if not using audio input)
#define AUDIO_LOW_LEVEL      0            // Audio low level adjustment(Remove if not using audio input)
