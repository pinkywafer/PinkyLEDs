# PinkyLEDs
MQTT and E1.31 pixel driver for ESP8266

Originally forked from: https://github.com/Snipercaine/Holiday-LED-files.  This was expanded to add buttons to control on/off, colour selection, and effect selection (at the request of my daughter who likes to have as many ways to control anything as possible!!)

This will drive strips of LEDs using json MQTT messages.  The original effects and DrZzs effects have been incorporated.
By selecting the additional effect "E131", you can drive the LEDs using E1.31 such as from Xlights. (Note that E131 can ONLY be selected via MQTT.

Additionally, MQTT discovery for Home Assistant is incorporated, as well as last will and testament messages.

To set up, edit the config.h file to suit your setup, and upload via the Arduino IDE.

Additional dependancies which you will require are:
FastLED
ESPAsyncE131
ESP8266WiFi

Thanks to the original author, DrZzs for additional effects, http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/index.html for color palettes and anyone else I've missed!
