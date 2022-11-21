#pragma once

#define FIRMWARE_VERSION "2.0.0" // Try "V" on the Serial port for this!

// Lightshow modes by name
enum lightshow_modes {
    DUET_MODE, // -------- FFT with linear scaling
    DUET_MODE_LOG, // ---- FFT with log scaling
    BLOOM_MODE, // ------- Slow Bloom Mode
    BLOOM_MODE_FAST, // -- Fast Bloom Mode
    WAVEFORM_MODE, // ---- Waveform is shown using LED brightness
    VU_MODE, // ---------- Not a real VU for any measurement sake, just a dance-y LED bar
    VU_MODE_DOT, // ------ Alternate VU display mode - dot with motion blur

    NUM_MODES  // used to know the length of this list if it changes
};

#include <cstdint>
#include <cstring>
#include <cmath>
#include <driver/gpio.h>
#include <driver/i2s.h>
#include <esp_intr_alloc.h>
#include <soc/i2s_reg.h>
#include <esp_now.h>
#include <esp_wifi_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#include "hardware_config.h"

#include "Arduino.h"
#include "serial.h"
#include "fastled.h"
#include "idfsb_lfs.h"
#include "analog.h"
#include "digital.h"
#include "ledc.h"
#include "random.h"
#include "timer.h"
#include "yield.h"
#include "esp.h"
#include "wifi.h"

#include "constants.h"
#include "globals.h"
#include "led_utilities.h"
#include "bridge_fs.h"
#include "lightshow_modes.h"
#include "noise_cal.h"
#include "p2p.h"
#include "knobs.h"
#include "buttons.h"
#include "serial_menu.h"
#include "i2s.h"
#include "sweet_spot.h"
#include "ESP32_fft.h"
#include "fft_parser.h"
#include "system.h"

