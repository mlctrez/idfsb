#pragma once

#include <stdint.h>
#include <soc/soc_caps.h>
#include <esp_log.h>
#include <driver/ledc.h>
#include <esp_check.h>

#define LEDC_CHANNELS           (SOC_LEDC_CHANNEL_NUM)
#define LEDC_MAX_BIT_WIDTH      SOC_LEDC_TIMER_BIT_WIDE_NUM

#ifdef SOC_LEDC_SUPPORT_XTAL_CLOCK
#define LEDC_DEFAULT_CLK        LEDC_USE_XTAL_CLK
#else
#define LEDC_DEFAULT_CLK        LEDC_AUTO_CLK
#endif

uint8_t channels_resolution[LEDC_CHANNELS] = {0};

static __attribute__((unused)) const char *LEDC_TAG = "ledc";

#define LEDC_ARG_CHECK(a, param) ESP_RETURN_ON_FALSE(a, ESP_ERR_INVALID_ARG, LEDC_TAG, param " argument is invalid")

uint32_t ledcSetup(uint8_t chan, uint32_t freq, uint8_t bit_num) {
    if (chan >= LEDC_CHANNELS || bit_num > LEDC_MAX_BIT_WIDTH) {
        ESP_LOGE("ledcSetup", "No more LEDC channels available! (maximum %u) or bit width too big (maximum %u)",
                 LEDC_CHANNELS, LEDC_MAX_BIT_WIDTH);
        return 0;
    }

    ledc_mode_t group = static_cast<ledc_mode_t>(chan / 8);
    ledc_timer_t timer = static_cast<ledc_timer_t>((chan / 2) % 4);
    ledc_timer_bit_t duty_resolution = static_cast<ledc_timer_bit_t>(bit_num);

    ledc_timer_config_t ledc_timer = {
            .speed_mode       = group,
            .duty_resolution  = duty_resolution,
            .timer_num        = timer,
            .freq_hz          = freq,
            .clk_cfg          = LEDC_DEFAULT_CLK
    };
    if (ledc_timer_config(&ledc_timer) != ESP_OK) {
        ESP_LOGE("ledcSetup", "ledc setup failed!");
        return 0;
    }
    channels_resolution[chan] = bit_num;
    return ledc_get_freq(group, timer);
}

void ledcAttachPin(uint8_t pin, uint8_t chan) {
    if (chan >= LEDC_CHANNELS) {
        return;
    }
    uint8_t group = (chan / 8), channel = (chan % 8), timer = ((chan / 2) % 4);

    ledc_channel_config_t ledc_channel = {
            .gpio_num       = pin,
            .speed_mode     = static_cast<ledc_mode_t>(group),
            .channel        = static_cast<ledc_channel_t>(channel),
            .intr_type      = LEDC_INTR_DISABLE,
            .timer_sel      = static_cast<ledc_timer_t>(timer),
            .duty           = 0,
            .hpoint         = 0,
            .flags          = {
                    // TODO: check that this is correct
                    .output_invert = 0
            }

    };
    ledc_channel_config(&ledc_channel);
}


void ledcWrite(uint8_t chan, uint32_t duty) {
    if (chan >= LEDC_CHANNELS) {
        return;
    }
    uint8_t group = (chan / 8), channel = (chan % 8);

    //Fixing if all bits in resolution is set = LEDC FULL ON
    uint32_t max_duty = (1 << channels_resolution[chan]) - 1;

    if ((duty == max_duty) && (max_duty != 1)) {
        duty = max_duty + 1;
    }

    auto ledc_group = static_cast<ledc_mode_t>(group);
    auto ledc_chan = static_cast<ledc_channel_t>(channel);

    ledc_set_duty(ledc_group, ledc_chan, duty);
    ledc_update_duty(ledc_group, ledc_chan);
}

