#!/bin/sh

clang-tidy -header-filter=.* -p build.cmake include/sched/**/*.h library/**/*.cc
