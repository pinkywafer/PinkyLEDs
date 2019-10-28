# PinkyLEDs v0.11.0  [<a href="https://www.buymeacoffee.com/V3q9id4" target="_blank"><img src="https://www.buymeacoffee.com/assets/img/custom_images/purple_img.png" alt="Buy Me A Coffee" style="height: auto !important;width: auto !important;" ></a>](https://www.buymeacoffee.com/V3q9id4)

## MQTT and E1.31 pixel driver for ESP8266 and ESP32

For setup information and more, please see [the wiki](https://github.com/pinkywafer/PinkyLEDs/wiki)

Version 0.11.0 is a major refactor and breaks out all (except a few core effects) to a seperate .ino file.  An effect template is given at the end of the file to enable you to easily contribute your own effects.

## Features

### Fade now implemented on power on/off and all effect changes.

### New from v0.10.0 Audio reactive effects

### New from v0.9.0 Rotary Encoder Support for Brightness and effect Speed

* LED driver
* MQTT control and status using Json messages
* MQTT discovery for Home Assistant for easy set up
* MQTT group topic to allow control of multiple boards with single MQTT message
* Last will and testament MQTT messages to show online status
* Support for Home Assistant `flash` function
* OTA updates
* Status indication at boot... red - powered up, amber - wifi connected, green MQTT connected

## Optional

* Home Assistant "white value" slider can be used to control effect speed
* Physical button control
* Rotary encoder control for Brightness and Speed
* Original sketch effects and DrZzs custom effects [(from this video)](https://www.youtube.com/watch?v=6Y6jUM1OaYM&t=365s) included.
* E1.31 Unicast control (ie Xlights) - (note can ONLY be selected via MQTT)
* Audio reactive effects using Microphone Audio amplifier module

Thanks to the original authors, Bruh, Library authors, DrZzs for additional effects, [http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/index.html](http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/index.html) for color palettes, @bbacskay and anyone else I've missed!
