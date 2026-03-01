// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/FlexibleJobShopInstance.h>

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include <sched/types/ShopInstanceConcepts.h>
#include <sched/support/Json.h>

static_assert(sched::concepts::ShopInstance<sched::shop::FlexibleJobShopInstance>);

namespace sched::shop {

  FlexibleJobShopInstance import_fjsp_txt(const std::filesystem::path& filename)
  {
    std::ifstream input(filename);

    if (!input) {
      throw std::runtime_error("File not found: " + filename.string());
    }

    std::size_t machine_count = 0;
    std::size_t job_count = 0;

    for (std::string line; std::getline(input, line);) {
      if (line[0] == '#') {
        continue;
      }

      std::istringstream first;
      first.str(line);
      first >> job_count >> machine_count;
      break;
    }

    FlexibleJobShopData data = {};
    data.machines = machine_count;

    for (std::size_t i = 0; i < job_count; ++i) {
      std::string line;

      if (!std::getline(input, line)) {
        throw std::runtime_error("Missing data for job #" + std::to_string(i));
      }

      std::istringstream stream;
      stream.str(line);

      std::size_t operation_count = 0;
      stream >> operation_count;
      assert(operation_count > 0);

      FlexibleJobData job = {};
      std::size_t operation_machine_count = 0;

      while (stream >> operation_machine_count) {
        FlexibleOperationData operation = {};
        std::size_t machine = 0;
        Time processing = 0;

        for (std::size_t j = 0; j < operation_machine_count; ++j) {
          stream >> machine >> processing;
          operation.choices.emplace_back(to_machine(machine), processing);
        }

        job.operations.push_back(std::move(operation));
      }

      assert(job.operations.size() == operation_count);
      data.jobs.push_back(std::move(job));
    }

    return { std::move(data) };
  }

  FlexibleJobShopInstance import_fjsp_json(const std::filesystem::path& filename)
  {
    FlexibleJobShopData data;

    std::ifstream stream(filename);
    Json root;
    stream >> root;

    root.get_to(data);

    return data;
  }

  void export_fjsp_json(const std::filesystem::path& filename, const FlexibleJobShopInstance& instance)
  {
    FlexibleJobShopData data = instance.data();

    Json root;
    root = data;

    std::ofstream stream(filename);
    dump_json(stream, root);
  }

}
