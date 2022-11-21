#pragma once

#include <fcntl.h>
#include <cstdio>

const int HEX = 0x10;

extern int unistd_read(int fd, void *buf, size_t len);

class SerialMock {
private:
  char serial_buf[1];
public:
  void println(const char *data) { printf("%s\n", data); };

  void println(uint32_t data) { printf("%lu\n", data); };

  void println(char *data) { printf("%s\n", data); };

  void println() { printf("\n"); };

  void print(char *data) { printf("%s", data); };

  void print(uint8_t data, int type) {
    switch (type) {
      case HEX:
        printf("%x", data);
    }
  };

  void print(const char *data) { printf("%s", data); };

  void print(uint8_t data) { printf("%d", data); };

  void begin(uint64_t baud_rate) {
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
  };

  int available() {
    return unistd_read(0, serial_buf, 1);
  }

  char read() {
    return serial_buf[0];
  }

  explicit operator bool() const {
    return true;
  }

};


extern SerialMock Serial;
