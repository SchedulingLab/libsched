// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_VEHICLE_LIST_VEHICLE_ASSIGNMENT_H
#define SCHED_SHOP_VEHICLE_LIST_VEHICLE_ASSIGNMENT_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/meta/Instance.h>
#include <sched/shop/input/VehicleListInput.h>
#include <sched/types/AssignmentTraits.h>

namespace sched::shop {

  struct SCHED_API VehicleListVehicleAssignment {
    using Input = VehicleListInput;

    template<typename Instance>
    std::vector<VehicleId> operator()([[maybe_unused]] const Instance& instance, const VehicleListInput& input)
    {
      return input;
    }
  };

}

namespace sched {

  template<>
  struct AssignmentTraits<shop::VehicleListVehicleAssignment> {
    static std::string name()
    {
      return "veh";
    }
  };

}

#endif // SCHED_SHOP_VEHICLE_LIST_VEHICLE_ASSIGNMENT_H
