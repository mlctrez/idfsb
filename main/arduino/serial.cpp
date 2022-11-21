#include "serial.h"
#include <unistd.h>

SerialMock Serial;

int unistd_read(int fd, void *buf, size_t len) {
  return ::read(fd, buf, len);
}