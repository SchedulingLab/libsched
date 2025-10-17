// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/FlexibleJobShopData.h>

#include <nlohmann/json.hpp>

namespace sched::shop {

  /*
   * FlexibleOperationData
   */

  void from_json(const Json& json, FlexibleOperationData& data)
  {
    json.get_to(data.choices);
  }

  void to_json(Json& json, const FlexibleOperationData& data)
  {
    json = data.choices;
  }

  /*
   * FlexibleJobData
   */

  void from_json(const Json& json, FlexibleJobData& data)
  {
    json.get_to(data.operations);
  }

  void to_json(Json& json, const FlexibleJobData& data)
  {
    json = data.operations;
  }

  /*
   * FlexibleJobShopData
   */

  void from_json(const Json& json, FlexibleJobShopData& data)
  {
    json.at("machines").get_to(data.machines);
    json.at("jobs").get_to(data.jobs);
  }

  void to_json(Json& json, const FlexibleJobShopData& data)
  {
    json = {
      { "machines", data.machines },
      { "jobs", data.jobs },
    };
  }

}
