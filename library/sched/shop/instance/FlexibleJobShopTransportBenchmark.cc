// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/FlexibleJobShopTransportBenchmark.h>

#include <fstream>

#include <nlohmann/json.hpp>

namespace sched::shop {

  void from_json(const nlohmann::json& j, FlexibleJobShopTransportBenchmark& benchmark) // NOLINT(misc-use-internal-linkage)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmark.name);
    benchmark.family = ""; // will be filled later
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);
    j.at("vehicles").get_to(benchmark.vehicles);

    std::string path_string;
    j.at("path").get_to(path_string);
    benchmark.path = path_string;
  }

  struct FlexibleJobShopTransportBenchmarkSet {
    std::string name;
    std::vector<FlexibleJobShopTransportBenchmark> instances;
  };

  void from_json(const nlohmann::json& j, FlexibleJobShopTransportBenchmarkSet& benchmarks) // NOLINT(misc-use-internal-linkage)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmarks.name);
    j.at("instances").get_to(benchmarks.instances);
  }

  std::vector<FlexibleJobShopTransportBenchmark> load_fjspt_benchmarks(const std::filesystem::path& filename)
  {
    std::vector<FlexibleJobShopTransportBenchmarkSet> families;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(families);

    std::vector<FlexibleJobShopTransportBenchmark> benchmarks;

    for (FlexibleJobShopTransportBenchmarkSet& family : families) {
      for (FlexibleJobShopTransportBenchmark& benchmark : family.instances) {
        benchmark.family = family.name;
        benchmarks.push_back(std::move(benchmark));
      }
    }

    return benchmarks;
  }

}
