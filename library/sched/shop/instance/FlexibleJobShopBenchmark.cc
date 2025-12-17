// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/FlexibleJobShopBenchmark.h>

#include <cassert>

#include <fstream>

#include <nlohmann/json.hpp>

namespace sched::shop {

  // NOLINTNEXTLINE(misc-use-internal-linkage)
  void from_json(const nlohmann::json& j, FlexibleJobShopBenchmark& benchmark)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);

    if (j.at("optimum").is_null()) {
      benchmark.optimum = 0;
      const auto& bounds = j.at("bounds");

      if (bounds.is_null()) {
        benchmark.upper_bound = 0;
        benchmark.lower_bound = 0;
      } else {
        j.at("bounds").at("upper").get_to(benchmark.upper_bound);
        j.at("bounds").at("lower").get_to(benchmark.lower_bound);
      }
    } else {
      j.at("optimum").get_to(benchmark.optimum);
      benchmark.upper_bound = benchmark.optimum;
      benchmark.lower_bound = benchmark.optimum;
    }

    std::string path_string;
    j.at("path").get_to(path_string);
    benchmark.path = path_string;
  }

  std::vector<FlexibleJobShopBenchmark> load_fjsp_benchmarks(const std::filesystem::path& filename)
  {
    std::vector<FlexibleJobShopBenchmark> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }

}
