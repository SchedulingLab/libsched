// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLOAT_LIST_VEHICLE_ASSIGNMENT_H
#define SCHED_SHOP_FLOAT_LIST_VEHICLE_ASSIGNMENT_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/meta/input/FloatListInput.h>
#include <sched/types/AssignmentTraits.h>

namespace sched::shop {

  struct SCHED_API FloatListVehicleAssignment {
    using Input = FloatListInput;

    template<typename Instance>
    std::vector<VehicleId> operator()(const Instance& instance, const FloatListInput& input)
    {
      const std::size_t vehicle_count = instance.vehicle_count();
      std::vector<VehicleId> assignment;

      for (auto value : input) {
        auto id = static_cast<std::size_t>(value * vehicle_count);
        assert(id < vehicle_count);
        assignment.push_back(VehicleId{ id });
      }

      return assignment;
    }
  };

}

namespace sched {

  template<>
  struct AssignmentTraits<shop::FloatListVehicleAssignment> {
    static std::string name()
    {
      return "ftr";
    }
  };

}

#endif // SCHED_SHOP_FLOAT_LIST_TRANSPORTATION_ASSIGNMENT_H
