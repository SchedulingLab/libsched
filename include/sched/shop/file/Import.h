// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_IMPORT_H
#define SCHED_IMPORT_H

#include <cstdint>

#include <filesystem>

#include <sched/Api.h>
#include <sched/Time.h>
#include <sched/shop/instance/FlexibleJobShopInstance.h>
#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>
#include <sched/shop/instance/JobShopInstance.h>

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

  struct SCHED_API FlexibleJobShopBenchmark {
    std::string name;
    int jobs;
    int machines;
    Time optimum;
    Time upper_bound;
    Time lower_bound;
    std::filesystem::path path;
  };

  struct SCHED_API FlexibleJobShopTransportBenchmark {
    std::string name;
    int jobs;
    int machines;
    int vehicles;
    std::filesystem::path path;
  };

  struct SCHED_API Import {
    static std::vector<JobShopBenchmark> load_jsp_benchmarks(const std::filesystem::path& filename);

    static std::vector<FlexibleJobShopBenchmark> load_fjsp_benchmarks(const std::filesystem::path& filename);
  };

}

#endif // SCHED_IMPORT_H
