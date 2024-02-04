#!/bin/sh

clang-tidy -p build include/sched/**/*.h library/**/*.cc
