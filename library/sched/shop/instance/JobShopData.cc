// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/JobShopData.h>

#include <nlohmann/json.hpp>

namespace sched::shop {

  /*
   * OperationData
   */

  void from_json(const Json& json, OperationData& data)
  {
    json.at("machine").get_to(data.machine);
    json.at("processing").get_to(data.processing);
  }

  void to_json(Json& json, const OperationData& data)
  {
    json = { { "machine", data.machine }, { "processing", data.processing } };
  }

  /*
   * JobData
   */

  void from_json(const Json& json, JobData& data)
  {
    json.get_to(data.operations);
  }

  void to_json(Json& json, const JobData& data)
  {
    json = data.operations;
  }

  /*
   * JobShopData
   */

  void from_json(const Json& json, JobShopData& data)
  {
    json.at("machines").get_to(data.machines);
    json.at("jobs").get_to(data.jobs);
  }

  void to_json(Json& json, const JobShopData& data)
  {
    json = { { "machines", data.machines }, { "jobs", data.jobs } };
  }

}
