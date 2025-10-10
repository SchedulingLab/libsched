// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/HoistSchedulingData.h>

namespace sched::shop {

  void from_json(const Json& json, HoistSchedulingOperationData& data)
  {
    const Json& processing = json.at("processing");
    processing.at("lo").get_to(data.processing.lo);

    if (processing.contains("hi")) {
      processing.at("hi").get_to(data.processing.hi);
    } else {
      data.processing.hi = TimeMax;
    }

    json.at("transport").get_to(data.transport);
  }

  void to_json(Json& json, const HoistSchedulingOperationData& data)
  {
    json = {
      { "processing", { "lo", data.processing.lo } },
      { "transport", data.transport },
    };

    if (data.processing.hi != TimeMax) {
      json.at("processing").emplace("hi", data.processing.hi);
    }
  }

  void from_json(const Json& json, HoistSchedulingData& data)
  {
    json.at("machines").get_to(data.machines);
    json.at("operations").get_to(data.operations);
    json.at("vehicles").get_to(data.vehicles);
    json.at("empty").get_to(data.empty);
  }

  void to_json(Json& json, const HoistSchedulingData& data)
  {
    json = {
      { "machines", data.machines },
      { "operations", data.operations },
      { "vehicles", data.vehicles },
      { "empty", data.empty },
    };
  }

}
