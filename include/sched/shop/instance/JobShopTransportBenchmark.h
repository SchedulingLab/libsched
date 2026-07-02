// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_BENCHMARK_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_BENCHMARK_H

#include <filesystem>
#include <string>
#include <vector>

#include <sched/Api.h>

namespace sched::shop {

  struct SCHED_API JobShopTransportBenchmark {
    std::string name;
    std::string family;
    int jobs;
    int machines;
    int vehicles;
    std::filesystem::path path;
  };

  SCHED_API std::vector<JobShopTransportBenchmark> load_jspt_benchmarks(const std::filesystem::path& filename);

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_BENCHMARK_H
