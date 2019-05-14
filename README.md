# THIS IS A DEVELOPMENT BRANCH AND NOT YET READY FOR DEPLOYMENT

# PinkyLEDs
## MQTT and E1.31 pixel driver for ESP8266 on D1 mini

Originally forked from: https://github.com/Snipercaine/Holiday-LED-files.  This was expanded to add buttons to control on/off, colour selection, and effect selection (at the request of my daughter who likes to have as many ways to control anything as possible!!)
### Features:
* LED driver
* MQTT control and status using Json messages
* Physical button control
* MQTT discovery for Home Assistant for easy set up
* MQTT group topic to allow control of multiple boards with single MQTT message
* Last will and testament MQTT messages to show online status
* Original sketch effects and DrZzs custom effects [(from this video)](https://www.youtube.com/watch?v=6Y6jUM1OaYM&t=365s) included.
* Home Assistant "white value" slider can be used to control effect speed
* E1.31 Unicast control (ie Xlights) - (note can ONLY be selected via MQTT)
* Support for Home Assistant `flash` function
* OTA updates
* Status indication at boot... red - powered up, amber - wifi connected, green MQTT connected


To set up, edit the config.h file to suit your setup, and upload via the Arduino IDE.

It should be possible to run this on other ESP8266 boards, but other changes may be needed

Additional libraries which you will require are:
* ESP8266 Boards Version 2.5.0 (installed to arduinoIDE in boards manager)
  * NOTE FastLED library is NOT compatible with ESP8266 boards version 2.5.1
* ArduinoJson version 6+
* PubSubClient version 2.7
* FastLED version 3.1+ (I'm currently using 3.2.6)
* ESPAsyncE131 version 1

Thanks to the original authors, DrZzs for additional effects, http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/index.html for color palettes and anyone else I've missed!

The example.yaml is provided as a guide if you choose not to use discovery
