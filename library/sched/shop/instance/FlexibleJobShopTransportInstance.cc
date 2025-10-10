// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>

#include <nlohmann/json.hpp>

#include <fstream>

#include <sched/types/ShopInstanceConcepts.h>
#include <sched/support/Json.h>

static_assert(sched::concepts::ShopTransportInstance<sched::shop::FlexibleJobShopTransportInstance>);

namespace sched::shop {

  FlexibleJobShopTransportInstance import_fjspt_json(const std::filesystem::path& filename, TransportationMode mode)
  {
    FlexibleJobShopTransportData data;

    std::ifstream stream(filename);
    Json root;
    stream >> root;

    root.get_to(data);

    return { std::move(data), mode };
  }

  void export_fjspt_json(const std::filesystem::path& filename, const FlexibleJobShopTransportInstance& instance)
  {
    FlexibleJobShopTransportData data = instance.data();
    const TransportationMode mode = instance.mode();

    if (mode == TransportationMode::Load || mode == TransportationMode::LoadUnload) {
      for (auto& job : data.jobs) {
        job.operations.erase(job.operations.begin());
      }
    }

    if (mode == TransportationMode::Unload || mode == TransportationMode::LoadUnload) {
      for (auto& job : data.jobs) {
        job.operations.pop_back();
      }
    }

    Json root;
    root = data;

    std::ofstream stream(filename);
    dump_json(stream, root);
  }
}
