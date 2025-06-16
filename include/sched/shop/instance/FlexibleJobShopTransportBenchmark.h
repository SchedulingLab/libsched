// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_BENCHMARK_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_BENCHMARK_H

#include <filesystem>
#include <string>
#include <vector>

#include <sched/Api.h>

namespace sched::shop {

  struct SCHED_API FlexibleJobShopTransportBenchmark {
    std::string name;
    int jobs;
    int machines;
    int vehicles;
    std::filesystem::path path;
  };

  struct SCHED_API FlexibleJobShopTransportBenchmarkSet {
    std::string name;
    std::vector<FlexibleJobShopTransportBenchmark> instances;
  };

  SCHED_API std::vector<FlexibleJobShopTransportBenchmarkSet> load_fjspt_benchmarks(const std::filesystem::path& filename);

}

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_BENCHMARK_H
