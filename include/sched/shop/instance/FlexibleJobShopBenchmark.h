// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_BENCHMARK_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_BENCHMARK_H

#include <filesystem>
#include <string>
#include <vector>

#include <sched/Time.h>

namespace sched::shop {

  struct SCHED_API FlexibleJobShopBenchmark {
    std::string name;
    int jobs;
    int machines;
    Time optimum;
    Time upper_bound;
    Time lower_bound;
    std::filesystem::path path;
  };

  std::vector<FlexibleJobShopBenchmark> load_fjsp_benchmarks(const std::filesystem::path& filename);

}

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_BENCHMARK_H
