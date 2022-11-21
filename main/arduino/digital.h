#pragma once

#include <esp_log.h>
#include <esp_check.h>
#include <driver/gpio.h>

// arduino defs from arduino-esp32/cores/esp32/esp32-hal-gpio.h
#define LOW               0x0
#define HIGH              0x1

#define INPUT             0x01
#define OUTPUT            0x03
#define PULLUP            0x04
#define INPUT_PULLUP      0x05
#define PULLDOWN          0x08
#define INPUT_PULLDOWN    0x09
#define OPEN_DRAIN        0x10
#define OUTPUT_OPEN_DRAIN 0x12
#define ANALOG            0xC0


// from arduino-esp32/cores/esp32/esp32-hal-gpio.c
void pinMode(uint8_t pin, uint8_t mode) {
    if (!GPIO_IS_VALID_GPIO(pin)) {
        ESP_LOGE("pinMode", "Invalid pin selected %u", pin);
        return;
    }

    gpio_config_t conf = {
            .pin_bit_mask = (1ULL << pin),               /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
            .mode = GPIO_MODE_DISABLE,                   /*!< GPIO mode: set input/output mode                     */
            .pull_up_en = GPIO_PULLUP_DISABLE,           /*!< GPIO pull-up                                         */
            .pull_down_en = GPIO_PULLDOWN_DISABLE,       /*!< GPIO pull-down                                       */
            .intr_type = GPIO_INTR_DISABLE               /*!< GPIO interrupt type - previously set                 */
    };

    if (mode < 0x20) { //io
        conf.mode = static_cast<gpio_mode_t>(mode & (INPUT | OUTPUT));
        if (mode & OPEN_DRAIN) {
            conf.mode = static_cast<gpio_mode_t>(conf.mode | GPIO_MODE_DEF_OD);
        }
        if (mode & PULLUP) {
            conf.pull_up_en = GPIO_PULLUP_ENABLE;
        }
        if (mode & PULLDOWN) {
            conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
        }
    }
    if (gpio_config(&conf) != ESP_OK) {
        ESP_LOGE("digital.pinMode", "can't set pin %d to mode %d", pin, mode);
    }
}

//void digitalWrite(uint8_t pin, uint8_t val) {
//    gpio_set_level(static_cast<gpio_num_t>(pin), val);
//};

int digitalRead(uint8_t pin) {
    return gpio_get_level(static_cast<gpio_num_t>(pin));
};
