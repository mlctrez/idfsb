#ifndef IDFSB_TIMER_H
#define IDFSB_TIMER_H

#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

int64_t millis() {
    return esp_timer_get_time() / 1000;
}

int64_t micros() {
    return esp_timer_get_time();
}

void delay(uint32_t ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
#endif //IDFSB_TIMER_H
