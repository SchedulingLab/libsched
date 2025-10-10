// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_DATA_H
#define SCHED_SHOP_JOB_SHOP_DATA_H

#include <vector>

#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/support/Array2D.h>
#include <sched/support/Json.h>

namespace sched::shop {

  struct SCHED_API OperationData {
    MachineId machine = NoMachine;
    Time processing = 0;
  };

  SCHED_API void from_json(const Json& json, OperationData& data);
  SCHED_API void to_json(Json& json, const OperationData& data);

  struct SCHED_API JobData {
    std::vector<OperationData> operations;
  };

  SCHED_API void from_json(const Json& json, JobData& data);
  SCHED_API void to_json(Json& json, const JobData& data);

  struct SCHED_API JobShopData {
    std::size_t machines = 0;
    std::vector<JobData> jobs;
  };

  SCHED_API void from_json(const Json& json, JobShopData& data);
  SCHED_API void to_json(Json& json, const JobShopData& data);

  struct SCHED_API FlexibleOperationData {
    std::vector<OperationData> choices;
  };

  SCHED_API void from_json(const Json& json, FlexibleOperationData& data);
  SCHED_API void to_json(Json& json, const FlexibleOperationData& data);

  struct SCHED_API FlexibleJobData {
    std::vector<FlexibleOperationData> operations;
  };

  SCHED_API void from_json(const Json& json, FlexibleJobData& data);
  SCHED_API void to_json(Json& json, const FlexibleJobData& data);

  struct SCHED_API FlexibleJobShopData {
    std::size_t machines = 0;
    std::vector<FlexibleJobData> jobs;
  };

  SCHED_API void from_json(const Json& json, FlexibleJobShopData& data);
  SCHED_API void to_json(Json& json, const FlexibleJobShopData& data);

  struct SCHED_API FlexibleJobShopTransportData {
    std::size_t machines = 0;
    std::size_t stations = 0;
    MachineId load = NoMachine;
    MachineId unload = NoMachine;
    std::vector<FlexibleJobData> jobs;
    std::size_t vehicles = 0;
    Array2D<Time> empty;
    Array2D<Time> loaded;
  };

  SCHED_API void from_json(const Json& json, FlexibleJobShopTransportData& data);
  SCHED_API void to_json(Json& json, const FlexibleJobShopTransportData& data);

}

#endif // SCHED_SHOP_JOB_SHOP_DATA_H
