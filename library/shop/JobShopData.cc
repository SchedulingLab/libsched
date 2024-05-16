#include <sched/shop/JobShopData.h>

namespace sched {

  void from_json(const nlohmann::json& json, Array2D<Time>& data)
  {
    assert(json.is_array());

    for (std::size_t i = 0; i < data.rows(); ++i) {
      for (std::size_t j = 0; j < data.cols(); ++j) {
        json[i][j].get_to(data(i, j));
      }
    }
  }

}

namespace sched::shop {

  void from_json(const nlohmann::json& json, OperationData& data)
  {
    json.at("machine").get_to(data.machine);
    json.at("processing").get_to(data.processing);
  }

  void from_json(const nlohmann::json& json, FlexibleOperationData& data)
  {
    json.get_to(data.choices);
  }

  void from_json(const nlohmann::json& json, FlexibleJobData& data)
  {
    json.get_to(data.operations);
  }

  void from_json(const nlohmann::json& json, FlexibleJobShopTransportData& data)
  {
    json.at("machines").get_to(data.machines);
    json.at("load").get_to(data.load);
    json.at("unload").get_to(data.unload);
    json.at("jobs").get_to(data.jobs);
    json.at("vehicles").get_to(data.vehicles);
    data.empty = data.loaded = Array2D<Time>(data.machines, data.machines);
    json.at("empty").get_to(data.empty);
    json.at("loaded").get_to(data.loaded);
  }

}
