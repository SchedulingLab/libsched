// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_DATA_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_DATA_H

#include <vector>

#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/shop/instance/FlexibleJobShopData.h>
#include <sched/support/Array2D.h>
#include <sched/support/Json.h>

namespace sched::shop {

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

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_DATA_H
