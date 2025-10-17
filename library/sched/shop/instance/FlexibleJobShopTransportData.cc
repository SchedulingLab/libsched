// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/FlexibleJobShopTransportData.h>

#include <nlohmann/json.hpp>

namespace sched::shop {

  /*
   * FlexibleJobShopTransportData
   */

  void from_json(const Json& json, FlexibleJobShopTransportData& data)
  {
    json.at("machines").get_to(data.machines);
    json.at("stations").get_to(data.stations);
    json.at("load").get_to(data.load);
    json.at("unload").get_to(data.unload);
    json.at("jobs").get_to(data.jobs);
    json.at("vehicles").get_to(data.vehicles);
    const std::size_t device_count = data.machines + data.stations;
    data.empty = data.loaded = Array2D<Time>(device_count, device_count);
    json.at("empty").get_to(data.empty);
    json.at("loaded").get_to(data.loaded);
  }

  void to_json(Json& json, const FlexibleJobShopTransportData& data)
  {
    json = {
      { "machines", data.machines },
      { "stations", data.stations },
      { "load", data.load },
      { "unload", data.unload },
      { "jobs", data.jobs },
      { "vehicles", data.vehicles },
      { "empty", data.empty },
      { "loaded", data.loaded },
    };
  }
}
