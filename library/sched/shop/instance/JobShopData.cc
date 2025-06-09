// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/JobShopData.h>

namespace sched {

  void from_json(const Json& json, Array2D<Time>& data)
  {
    assert(json.is_array());
    assert(json.size() == data.rows());

    for (std::size_t i = 0; i < data.rows(); ++i) {
      assert(json[i].is_array());
      assert(json[i].size() == data.cols());

      for (std::size_t j = 0; j < data.cols(); ++j) {
        json[i][j].get_to(data(i, j));
      }
    }
  }

  void to_json(Json& json, const Array2D<Time>& data)
  {
    json = Json::array();

    for (std::size_t i = 0; i < data.rows(); ++i) {
      Json row_json = Json::array();

      for (std::size_t j = 0; j < data.cols(); ++j) {
        row_json.push_back(data[i][j]);
      }

      json.push_back(std::move(row_json));
    }
  }

}

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
