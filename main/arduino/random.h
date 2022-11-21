#pragma once

#include <cstdlib>
#include "esp_random.h"

void randomSeed(unsigned long seed) {
    if (seed != 0) {
        srand(seed);
    }
}

long random(long howBig) {
    uint32_t x = esp_random();
    uint64_t m = uint64_t(x) * uint64_t(howBig);
    uint32_t l = uint32_t(m);
    if (l < howBig) {
        uint32_t t = -howBig;
        if (t >= howBig) {
            t -= howBig;
            if (t >= howBig)
                t %= howBig;
        }
        while (l < t) {
            x = esp_random();
            m = uint64_t(x) * uint64_t(howBig);
            l = uint32_t(m);
        }
    }
    return m >> 32;
}

long random(long howSmall, long howBig) {
    if (howSmall >= howBig) {
        return howSmall;
    }
    long diff = howBig - howSmall;
    return random(diff) + howSmall;
}