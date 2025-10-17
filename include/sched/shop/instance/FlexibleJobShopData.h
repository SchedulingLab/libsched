// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_DATA_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_DATA_H

#include <vector>

#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/shop/instance/JobShopData.h>
#include <sched/support/Json.h>

namespace sched::shop {

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

}

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_DATA_H
