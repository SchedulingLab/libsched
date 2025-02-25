// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/JobShopInstance.h>

#include <fstream>
#include <sstream>

#include <sched/types/ShopInstanceConcepts.h>

static_assert(sched::concepts::ShopInstance<sched::shop::JobShopInstance>);

namespace sched::shop {

  JobShopInstance import_jsp_txt(const std::filesystem::path& filename)
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

    JobShopData data = {};
    data.machines = machine_count;

    for (std::string line; std::getline(input, line);) {
      std::istringstream stream;
      stream.str(line);

      JobData job = {};
      std::size_t machine = 0;
      Time processing = 0;

      while (stream >> machine >> processing) {
        job.operations.emplace_back(MachineId{ machine }, processing);
      }

      data.jobs.push_back(std::move(job));
    }

    assert(data.jobs.size() == job_count);

    return { std::move(data) };
  }

}
