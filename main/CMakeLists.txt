idf_component_register(
        SRCS
            "app_main.cpp"
            "firmware/ESP32_fft.cpp"
            "encoder/led_strip_encoder.c"
            "arduino/serial.cpp"
        INCLUDE_DIRS
            "overrides"
            "firmware"
            "encoder"
            "arduino"
            "."

)
