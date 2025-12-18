// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_SCHEDULING_DATA_H
#define SCHED_SHOP_HOIST_SCHEDULING_DATA_H

#include <cstdint>

#include <vector>

#include <nlohmann/json.hpp>

#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/support/Array2D.h>
#include <sched/support/Json.h>

namespace sched::shop {

  struct SCHED_API HoistSchedulingOperationData {
    TimeWindow processing = {};
    std::size_t machines = 1;
    Time transport = 0;
  };

  SCHED_API void from_json(const Json& json, HoistSchedulingOperationData& data);
  SCHED_API void to_json(Json& json, const HoistSchedulingOperationData& data);

  struct SCHED_API HoistSchedulingData {
    std::size_t machines = 0;
    std::vector<HoistSchedulingOperationData> operations;
    Array2D<Time> empty;
  };

  SCHED_API void from_json(const Json& json, HoistSchedulingData& data);
  SCHED_API void to_json(Json& json, const HoistSchedulingData& data);

  enum class HoistMachineType : uint8_t {
    Single,
    Multi
  };

  enum class HoistCarrierConstraint : uint8_t {
    NoRestriction,
    Circulation,
  };

  struct SCHED_API HoistSchedulingVariant {
    std::size_t vehicles = 1;
    HoistMachineType machine_type = HoistMachineType::Single;
    HoistCarrierConstraint carrier_constraint = HoistCarrierConstraint::Circulation;
  };

}

#endif // SCHED_SHOP_HOIST_SCHEDULING_DATA_H
