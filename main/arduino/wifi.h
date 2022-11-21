#pragma once

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_check.h"

class WiFiMock {
public:
  esp_err_t mode(wifi_mode_t mode) {


    ESP_RETURN_ON_ERROR(esp_netif_init(), "wifi.h", "failed on esp_netif_init");
    ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), "wifi.h", "failed on esp_event_loop_create_default");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(nvs_flash_init(), "wifi.h", "failed on nvs_flash_init");
    esp_err_t initErr = esp_wifi_init(&cfg);
    ESP_RETURN_ON_ERROR(initErr, "wifi.h", "failed on esp_wifi_init %s", esp_err_to_name(initErr));
    ESP_RETURN_ON_ERROR(esp_wifi_set_storage(WIFI_STORAGE_RAM), "wifi.h", "failed on esp_wifi_set_storage");
    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(mode), "wifi.h", "failed on esp_wifi_set_mode");
    ESP_RETURN_ON_ERROR(esp_wifi_start(), "wifi.h", "failed on esp_wifi_start");

    return ESP_OK;
  }
};

WiFiMock WiFi;

