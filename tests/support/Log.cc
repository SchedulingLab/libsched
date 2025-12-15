// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <gtest/gtest.h>

#include <sched/support/Log.h>

TEST(Log, Simple) {
  sched::Log::println("Hello World!");
}
