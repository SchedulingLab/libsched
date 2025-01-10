#ifndef SCHED_JOB_SHOP_DATA_H
#define SCHED_JOB_SHOP_DATA_H

#include <vector>

#include <nlohmann/json.hpp>

#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/support/Array2D.h>

namespace sched::shop {

  struct OperationData {
    MachineId machine = NoMachine;
    Time processing = 0;
  };

  struct FlexibleOperationData {
    std::vector<OperationData> choices;
  };

  struct FlexibleJobData {
    std::vector<FlexibleOperationData> operations;
  };

  struct FlexibleJobShopTransportData {
    std::size_t machines = 0;
    MachineId load = NoMachine;
    MachineId unload = NoMachine;
    std::vector<FlexibleJobData> jobs;
    std::size_t vehicles = 0;
    Array2D<Time> empty;
    Array2D<Time> loaded;
  };

  void from_json(const nlohmann::json& json, FlexibleJobShopTransportData& data);

}

#endif // SCHED_JOB_SHOP_DATA_H
