#pragma once

// code from arduino-esp32/cores/esp32/esp32-hal-adc.c

#include <cstdint>
#include <esp_log.h>
#include <driver/adc.h>
#include <soc/adc_periph.h>
#include <soc/rtc_io_reg.h>
#include <soc/dac_channel.h>
#include "digital.h"

#define DEFAULT_VREF    1100

typedef enum {
    ADC_0db,
    ADC_2_5db,
    ADC_6db,
    ADC_11db,
    ADC_ATTENDB_MAX
} adc_attenuation_t;

static uint8_t __analogAttenuation = ADC_11db; // 11db

// original code caused __analogWidth to be 13 or 14 not 3 or 4
// static uint8_t __analogWidth = ADC_WIDTH_MAX - 1; //3 for ESP32/ESP32C3; 4 for ESP32S2

#if defined CONFIG_IDF_TARGET_ESP32 || defined CONFIG_IDF_TARGET_ESP32S3
static uint8_t __analogWidth = 3; //3 for ESP32/ESP32C3; 4 for ESP32S2
static uint8_t __analogReturnedWidth = 12; //12 for ESP32/ESP32C3; 13 for ESP32S2
#endif
#ifdef CONFIG_IDF_TARGET_ESP32S2
static uint8_t __analogWidth = 4; //3 for ESP32/ESP32C3; 4 for ESP32S2
static uint8_t __analogReturnedWidth = 13; //12 for ESP32/ESP32C3; 13 for ESP32S2
#endif

static uint8_t __analogClockDiv = 1;
static adc_attenuation_t __pin_attenuation[SOC_GPIO_PIN_COUNT];

int8_t digitalPinToAnalogChannel(uint8_t pin) {
    uint8_t channel = 0;
    if (pin < SOC_GPIO_PIN_COUNT) {
        for (uint8_t i = 0; i < SOC_ADC_PERIPH_NUM; i++) {
            for (uint8_t j = 0; j < SOC_ADC_MAX_CHANNEL_NUM; j++) {
                if (adc_channel_io_map[i][j] == pin) {
                    return channel;
                }
                channel++;
            }
        }
    }
    return -1;
}

static inline uint16_t mapResolution(uint16_t value) {
    uint8_t from = __analogWidth + 9;
    if (from == __analogReturnedWidth) {
        return value;
    }
    if (from > __analogReturnedWidth) {
        return value >> (from - __analogReturnedWidth);
    }
    return value << (__analogReturnedWidth - from);
}

void __analogSetClockDiv(uint8_t clockDiv) {
    if (!clockDiv) {
        clockDiv = 1;
    }
    __analogClockDiv = clockDiv;
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
    adc_set_clk_div(__analogClockDiv);
#endif
}

void __analogInit() {
    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;
    __analogSetClockDiv(__analogClockDiv);
#if CONFIG_IDF_TARGET_ESP32
    __analogSetWidth(__analogWidth + 9);//in bits
#endif
    for (int i = 0; i < SOC_GPIO_PIN_COUNT; i++) {
        __pin_attenuation[i] = ADC_ATTENDB_MAX;
    }
}

void __analogSetPinAttenuation(uint8_t pin, adc_attenuation_t attenuation) {
    int8_t channel = digitalPinToAnalogChannel(pin);
    if (channel < 0 || attenuation > 3) {
        return;
    }
    if (channel > (SOC_ADC_MAX_CHANNEL_NUM - 1)) {
        adc2_config_channel_atten(static_cast<adc2_channel_t>(channel - SOC_ADC_MAX_CHANNEL_NUM), static_cast<adc_atten_t>(attenuation));
    } else {
        adc1_config_channel_atten(static_cast<adc1_channel_t>(channel), static_cast<adc_atten_t>(attenuation));
    }
    __analogInit();
    if ((__pin_attenuation[pin] != ADC_ATTENDB_MAX) || (attenuation != __analogAttenuation)) {
        __pin_attenuation[pin] = attenuation;
    }
}

bool __adcAttachPin(uint8_t pin) {
    int8_t channel = digitalPinToAnalogChannel(pin);
    if (channel < 0) {
        ESP_LOGE("__adcAttachPin", "Pin %u is not ADC pin!", pin);
        return false;
    }
    __analogInit();
#if SOC_DAC_SUPPORTED
    if (pin == DAC_CHANNEL_1_GPIO_NUM) {
        CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_XPD_DAC | RTC_IO_PDAC1_DAC_XPD_FORCE);//stop dac1
    } else if (pin == DAC_CHANNEL_2_GPIO_NUM) {
        CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC2_REG, RTC_IO_PDAC2_XPD_DAC | RTC_IO_PDAC2_DAC_XPD_FORCE);//stop dac2
    }
#endif

//    pinMode(pin, ANALOG);
    if (__pin_attenuation[pin] != ADC_ATTENDB_MAX) {
        __analogSetPinAttenuation(pin, __pin_attenuation[pin]);
    } else {
        __analogSetPinAttenuation(pin, static_cast<adc_attenuation_t>(__analogAttenuation));
    }
    return true;
}


uint16_t analogRead(uint8_t pin) {
    int8_t channel = digitalPinToAnalogChannel(pin);
    int value = 0;
    esp_err_t r = ESP_OK;
    if (channel < 0) {
        ESP_LOGE("analogRead", "Pin %u is not ADC pin!", pin);
        return value;
    }
    __adcAttachPin(pin);
    if (channel > (SOC_ADC_MAX_CHANNEL_NUM - 1)) {
        channel -= SOC_ADC_MAX_CHANNEL_NUM;
        r = adc2_get_raw(static_cast<adc2_channel_t>(channel), static_cast<adc_bits_width_t>(__analogWidth), &value);
        if (r == ESP_OK) {
            return mapResolution(value);
        } else if (r == ESP_ERR_INVALID_STATE) {
            ESP_LOGE("analogRead", "GPIO%u: %s: ADC2 not initialized yet.", pin, esp_err_to_name(r));
        } else if (r == ESP_ERR_TIMEOUT) {
            ESP_LOGE("analogRead",
                     "GPIO%u: %s: ADC2 is in use by Wi-Fi. Please see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-limitations for more info",
                     pin, esp_err_to_name(r));
        } else {
            ESP_LOGE("analogRead", "GPIO%u: %s", pin, esp_err_to_name(r));
        }
    } else {
        value = adc1_get_raw(static_cast<adc1_channel_t>(channel));
        return mapResolution(value);
    }
    return mapResolution(value);
}


