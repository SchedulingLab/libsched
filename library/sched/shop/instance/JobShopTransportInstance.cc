// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/JobShopTransportInstance.h>

#include <nlohmann/json.hpp>

#include <fstream>

#include <sched/types/ShopInstanceConcepts.h>
#include <sched/support/Json.h>

static_assert(sched::concepts::ShopTransportInstance<sched::shop::JobShopTransportInstance>);

namespace sched::shop {

  JobShopTransportInstance import_jspt_json(const std::filesystem::path& filename, TransportationMode mode)
  {
    JobShopTransportData data;

    std::ifstream stream(filename);
    Json root;
    stream >> root;

    root.get_to(data);

    return { std::move(data), mode };
  }

  void export_jspt_json(const std::filesystem::path& filename, const JobShopTransportInstance& instance)
  {
    JobShopTransportData data = instance.data();
    const TransportationMode mode = instance.mode();

    if (mode == TransportationMode::Load || mode == TransportationMode::LoadUnload) {
      for (JobData& job : data.jobs) {
        job.operations.erase(job.operations.begin());
      }
    }

    if (mode == TransportationMode::Unload || mode == TransportationMode::LoadUnload) {
      for (JobData& job : data.jobs) {
        job.operations.pop_back();
      }
    }

    Json root;
    root = data;

    std::ofstream stream(filename);
    dump_json(stream, root);
  }
}
