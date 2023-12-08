#!/bin/sh

clang-tidy -header-filter=.* -p build include/sched/**/*.h library/**/*.cc
