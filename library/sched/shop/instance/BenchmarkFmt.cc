// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/BenchmarkFmt.h>

static_assert(std::formattable<sched::shop::JobShopBenchmark, char>);
static_assert(std::formattable<sched::shop::FlexibleJobShopBenchmark, char>);
static_assert(std::formattable<sched::shop::FlexibleJobShopTransportBenchmark, char>);
