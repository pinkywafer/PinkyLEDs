# PinkyLEDs
## MQTT and E1.31 pixel driver for ESP8266

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

Additional dependancies which you will require are:
ArduinoJson.h
ESP8266WiFi.h
PubSubClient.h
FastLED.h
ArduinoOTA.h
ESPAsyncE131.h

Thanks to the original authors, DrZzs for additional effects, http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/index.html for color palettes and anyone else I've missed!

The example.yaml is provided as a guide if you choose not to use discovery
