#pragma once

// makes compile of ESP32_fft.cpp happy
#include <cstdint>
#include <cstring>
#include <cmath>

// avoid duplicate definitions
#define SERIAL_EXTERNAL 1
#include "serial.h"

extern int64_t millis();
extern int64_t micros();
