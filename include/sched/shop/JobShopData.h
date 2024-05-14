#ifndef SCHED_JOB_SHOP_DATA_H
#define SCHED_JOB_SHOP_DATA_H

#include <vector>

#include <nlohmann/json.hpp>

#include <sched/common/Ids.h>
#include <sched/common/Time.h>
#include <sched/common/Array2D.h>

namespace sched::shop {

  struct OperationData {
    MachineId machine;
    Time processing;
  };

  struct FlexibleOperationData {
    std::vector<OperationData> choices;
  };

  struct FlexibleJobData {
    std::vector<FlexibleOperationData> operations;
  };

  struct FlexibleJobShopTransportData {
    std::size_t machines;
    std::vector<FlexibleJobData> jobs;
    std::size_t vehicles;
    Array2D<Time> empty;
    Array2D<Time> loaded;
  };

  void from_json(const nlohmann::json& json, FlexibleJobShopTransportData& data);

}

#endif // SCHED_JOB_SHOP_DATA_H
