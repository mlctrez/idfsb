#!/usr/bin/env bash

git submodule add --depth 1 https://github.com/espressif/arduino-esp32.git

(
  cd arduino-esp32 || fail
  git submodule update --init --recursive
  git fetch origin --depth 1 refs/tags/2.0.5:refs/tags/2.0.5
  git checkout 2.0.5
)
