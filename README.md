# idfsb

This project is an attempt to get [Sensory Bridge](https://github.com/connornishijima/SensoryBridge) to run with minimal modifications on
[esp-idf](https://github.com/espressif/esp-idf)  v5.x 

Specifically [this version](https://github.com/espressif/esp-idf/releases/tag/v5.0-rc1)
which has some advanced RMT and I2S features.

## Approach

* Use the existing source files as-is and build out minimal stubs for [Arduino](https://github.com/mlctrez/idfsb/tree/master/main/arduino) specific code
  * Serial.begin(), Serial.printXX(), Serial.available(), and Serial.read()
  * pinMode(), digitalRead(), analogRead(), millis(), micros()
  * use [esp_littlefs](https://github.com/joltwallet/esp_littlefs) to replace LittleFS
  * Use TinyUSB as a component from espressif's component library. 
  * FastLED replaced with minimal versions of CRGB/CHSV and related functions
  * Led encoder is pulled from an esp RMT example and only supports WS2812 leds

## Setting up

get esp-idf

```bash
mkdir -p ~/esp
cd ~/esp
git clone --depth 1 -b v5.0-rc1 --recursive https://github.com/espressif/esp-idf.git

cd ~/esp/esp-idf

git describe --long
v5.0-rc1-0-g3f2f35b

# run the installer for the target platform
./install.sh esp32s2

# source in one of the export.xx files, depending on your OS
. ~/esp/esp-idf/export.sh

# It is also helpful to setup these two environment variables.  
export IDF_TARGET=esp32s2
export ESPPORT=/dev/ttyACM0


```

Clone this repo, and submodules.

```bash
# 
git clone --recursive https://github.com/mlctrez/idfsb.git
```

Build the project using `idf.py build` and flash hardware using `idf.py flash monitor`

Note that
the [IDF monitor](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-monitor.html) allows
triggering builds and flashing the device without exiting the monitor.


```text
# sometimes this error pops up
Some components (joltwallet/littlefs) in the "managed_components" directory were modified         
on the disk since the last run of the CMake. Content of this directory is managed automatically.

Just remove managed_components/joltwallet__littlefs/.component_hash and continue


```