#pragma once

#include <cstdio>
#include "esp_system.h"

class ESPMock {
public:
    void restart() {
        esp_restart();
    }
};

ESPMock ESP;

