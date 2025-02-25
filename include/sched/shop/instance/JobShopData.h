// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_DATA_H
#define SCHED_SHOP_JOB_SHOP_DATA_H

#include <vector>

#include <nlohmann/json.hpp>

#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/support/Array2D.h>

namespace sched::shop {

  struct SCHED_API OperationData {
    MachineId machine = NoMachine;
    Time processing = 0;
  };

  SCHED_API void from_json(const nlohmann::json& json, OperationData& data);

  struct SCHED_API JobData {
    std::vector<OperationData> operations;
  };

  SCHED_API void from_json(const nlohmann::json& json, JobData& data);

  struct SCHED_API JobShopData {
    std::size_t machines = 0;
    std::vector<JobData> jobs;
  };

  SCHED_API void from_json(const nlohmann::json& json, JobShopData& data);

  struct SCHED_API FlexibleOperationData {
    std::vector<OperationData> choices;
  };

  SCHED_API void from_json(const nlohmann::json& json, FlexibleOperationData& data);

  struct SCHED_API FlexibleJobData {
    std::vector<FlexibleOperationData> operations;
  };

  SCHED_API void from_json(const nlohmann::json& json, FlexibleJobData& data);

  struct SCHED_API FlexibleJobShopData {
    std::size_t machines = 0;
    MachineId load = NoMachine;
    MachineId unload = NoMachine;
    std::vector<FlexibleJobData> jobs;
  };

  SCHED_API void from_json(const nlohmann::json& json, FlexibleJobShopData& data);

  struct SCHED_API FlexibleJobShopTransportData {
    std::size_t machines = 0;
    MachineId load = NoMachine;
    MachineId unload = NoMachine;
    std::vector<FlexibleJobData> jobs;
    std::size_t vehicles = 0;
    Array2D<Time> empty;
    Array2D<Time> loaded;
  };

  SCHED_API void from_json(const nlohmann::json& json, FlexibleJobShopTransportData& data);

}

#endif // SCHED_SHOP_JOB_SHOP_DATA_H
