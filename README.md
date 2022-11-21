# idfsb

This project is an attempt to get [Sensory Bridge](https://github.com/connornishijima/SensoryBridge) to run correctly on
[esp-idf](https://github.com/espressif/esp-idf).
Specifically [this version](https://github.com/espressif/esp-idf/releases/tag/v5.0-rc1)
which has some advanced RMT and I2S features.

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


