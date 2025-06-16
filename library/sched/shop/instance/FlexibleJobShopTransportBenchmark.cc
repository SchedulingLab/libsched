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
    j.at("jobs").get_to(benchmark.jobs);
    j.at("machines").get_to(benchmark.machines);
    j.at("vehicles").get_to(benchmark.vehicles);

    std::string path_string;
    j.at("path").get_to(path_string);
    benchmark.path = path_string;
  }

  void from_json(const nlohmann::json& j, FlexibleJobShopTransportBenchmarkSet& benchmarks) // NOLINT(misc-use-internal-linkage)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmarks.name);
    j.at("instances").get_to(benchmarks.instances);
  }

  std::vector<FlexibleJobShopTransportBenchmarkSet> load_fjspt_benchmarks(const std::filesystem::path& filename)
  {
    std::vector<FlexibleJobShopTransportBenchmarkSet> benchmarks;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(benchmarks);
    return benchmarks;
  }



}
