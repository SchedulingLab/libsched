// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_DATA_H
#define SCHED_SHOP_JOB_SHOP_DATA_H

#include <vector>

#include <sched/Ids.h>
#include <sched/Time.h>
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

}

#endif // SCHED_SHOP_JOB_SHOP_DATA_H
