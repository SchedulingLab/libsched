// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/JobShopTransportBenchmark.h>

#include <fstream>

#include <nlohmann/json.hpp>

namespace sched::shop {

  void from_json(const nlohmann::json& j, JobShopTransportBenchmark& benchmark) // NOLINT(misc-use-internal-linkage)
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

  struct JobShopTransportBenchmarkSet { // NOLINT(misc-use-internal-linkage)
    std::string name;
    std::vector<JobShopTransportBenchmark> instances;
  };

  void from_json(const nlohmann::json& j, JobShopTransportBenchmarkSet& benchmarks) // NOLINT(misc-use-internal-linkage)
  {
    assert(!j.is_null());
    j.at("name").get_to(benchmarks.name);
    j.at("instances").get_to(benchmarks.instances);
  }

  std::vector<JobShopTransportBenchmark> load_jspt_benchmarks(const std::filesystem::path& filename)
  {
    std::vector<JobShopTransportBenchmarkSet> families;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    assert(root.is_array());
    root.get_to(families);

    std::vector<JobShopTransportBenchmark> benchmarks;

    for (JobShopTransportBenchmarkSet& family : families) {
      for (JobShopTransportBenchmark& benchmark : family.instances) {
        benchmark.family = family.name;
        benchmarks.push_back(std::move(benchmark));
      }
    }

    return benchmarks;
  }

}


