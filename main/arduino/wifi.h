#pragma once

#include "esp_wifi.h"

class WiFiMock {
public:
    void mode(wifi_mode_t mode) {

//        ESP_ERROR_CHECK(esp_netif_init());
//        ESP_ERROR_CHECK(esp_event_loop_create_default());
//        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//        ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
//        ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
//        ESP_ERROR_CHECK( esp_wifi_set_mode(mode) );
//        ESP_ERROR_CHECK( esp_wifi_start());
    }
};

WiFiMock WiFi;

