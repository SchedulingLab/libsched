// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_BENCHMARK_H
#define SCHED_SHOP_JOB_SHOP_BENCHMARK_H

#include <cstdint>

#include <filesystem>
#include <string>
#include <vector>

#include <sched/Time.h>

namespace sched::shop {

  enum class BenchmarkGenerator : uint8_t {
    Unknown,
    Const,
    Uniform99,
    Uniform200,
    Binomial,
    NegativeBinomial,
  };

  struct SCHED_API JobShopBenchmark {
    std::string name;
    int jobs;
    int machines;
    Time optimum;
    Time upper_bound;
    Time lower_bound;
    BenchmarkGenerator generator;
    std::filesystem::path path;
  };

  std::vector<JobShopBenchmark> load_jsp_benchmarks(const std::filesystem::path& filename);

}

#endif // SCHED_SHOP_JOB_SHOP_BENCHMARK_H

