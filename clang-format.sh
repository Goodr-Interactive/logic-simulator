#!/usr/bin/env bash

clang-format -i src/*.c src/**/*.c include/**/*.h
clang-format -i tests/*.cpp
