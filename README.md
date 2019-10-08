# PinkyLEDs v0.9.4
## MQTT and E1.31 pixel driver for ESP8266 and ESP32
Known working boards:
* D1 mini
* NodeMCU 1.0
* DOIT ESP32 DEVKIT V1
* Espressif ESP32-WROOM-32D Dev board

Originally forked from: https://github.com/Snipercaine/Holiday-LED-files.  This was expanded to add buttons to control on/off, colour selection, and effect selection (at the request of my daughter who likes to have as many ways to control anything as possible!!)
# New to v0.9.0 Rotary Encoder Support for Brightness and effect Speed
### Features:
* LED driver
* MQTT control and status using Json messages
* MQTT discovery for Home Assistant for easy set up
* MQTT group topic to allow control of multiple boards with single MQTT message
* Last will and testament MQTT messages to show online status
* Support for Home Assistant `flash` function
* OTA updates
* Status indication at boot... red - powered up, amber - wifi connected, green MQTT connected
# Optional
* Home Assistant "white value" slider can be used to control effect speed
* Physical button control
* Rotary encoder control for Brightness and Speed
* Original sketch effects and DrZzs custom effects [(from this video)](https://www.youtube.com/watch?v=6Y6jUM1OaYM&t=365s) included.
* E1.31 Unicast control (ie Xlights) - (note can ONLY be selected via MQTT)

To set up, edit the config.h file to suit your setup, and upload via the Arduino IDE.

It should be possible to run this on other ESP8266 boards, but other changes may be needed

# Board libraries required:
* ESP8266 Boards Version 2.5.2 (installed to arduinoIDE in boards manager)

or
* ESP32 Boards Version 1.0.3 (installed to arduinoIDE in boards manager)
# Additional libraries required
* ArduinoJson version 5 (I'm using version 5.13.5) (Will not work with version 6)
* PubSubClient Latest version
* FastLED Latest version (3.3.2)
# Options
* ESPAsyncE131 Latest version (if you want to use E1.31)
* ESPAsyncUDP (required by ESPAsyncE131)
* RotaryEncoder by Matthias Hertel Latest Version (If you want to use rotary Encoder)

Thanks to the original authors, Library authors, DrZzs for additional effects, http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/index.html for color palettes, @bbacskay and anyone else I've missed!

The example.yaml is provided as a guide if you choose not to use discovery

[<a href="https://www.buymeacoffee.com/V3q9id4" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/purple_img.png" alt="Buy Me A Coffee" style="height: auto !important;width: auto !important;" ></a>](https://www.buymeacoffee.com/V3q9id4)
