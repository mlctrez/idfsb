#pragma once

#include <stdio.h>
#include "esp_log.h"
#include "esp_check.h"
#include "esp_littlefs.h"

const int FILE_READ = 0;
const int FILE_WRITE = 1;

const char *LFS_BASE_PATH = "/littlefs";

class File {
private:
  FILE *_file;

public:
  File(FILE *file);

  void seek(const int position) {
    fseek(_file, position, SEEK_SET);
  };

  void write(const uint8_t data) {
    fwrite(&data, sizeof(uint8_t), 1, _file);
  };

  int read() {
    uint8_t num;
    fread(&num, sizeof(uint8_t), 1, _file);
    return num;
  }

  void close() {
    fclose(_file);
  }

  explicit operator bool() const {
    return _file != nullptr;
  }

};

File::File(FILE *file) {
  _file = file;
}

class LittleFSMock {
public:
  bool begin(bool format) {

    esp_vfs_littlefs_conf_t conf = {
            .base_path = LFS_BASE_PATH,
            .partition_label = "littlefs",
            .format_if_mount_failed = format,
            .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    ESP_ERROR_CHECK_WITHOUT_ABORT(ret);

    return ret == ESP_OK;
  }


  bool remove(const char *data) {
    char buffer[50];
    sprintf(buffer, "%s%s", LFS_BASE_PATH, data);

    return std::remove(buffer) == 0;
  };

  File open(const char *data, int mode) {

    char buffer[50];
    sprintf(buffer, "%s%s", LFS_BASE_PATH, data);

    if (mode == FILE_READ) {
      return {fopen(buffer, "r")};
    } else if (mode == FILE_WRITE) {
      return {fopen(buffer, "w")};
    }

    return {nullptr};
  }
};

LittleFSMock LittleFS;

