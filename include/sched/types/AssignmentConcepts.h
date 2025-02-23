// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_ASSIGNMENT_CONCEPTS_H
#define SCHED_ASSIGNMENT_CONCEPTS_H

#include <concepts>
#include <map>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/types/AssignmentTraits.h>

namespace sched::concepts {

  namespace details {
    template<typename A, typename Instance, typename Input>
    concept BasicMachineAssignmentFor = requires(A assignment, Instance instance, Input input) {
      { assignment(instance, input) } -> std::same_as<std::map<OperationId, MachineId>>;
    };
  }

  template<typename A, typename Instance>
  concept MachineAssignmentFor = requires {
    typename A::Input;

    requires details::BasicMachineAssignmentFor<A, Instance, typename A::Input>;

    { AssignmentTraits<A>::name() } -> std::same_as<std::string>;
  };


  namespace details {
    template<typename A, typename Instance, typename Input>
    concept BasicVehicleAssignmentFor = requires(A assignment, Instance instance, Input input) {
      { assignment(instance, input) } -> std::same_as<std::vector<VehicleId>>;
    };
  }

  template<typename A, typename Instance>
  concept VehicleAssignmentFor = requires {
    typename A::Input;

    requires details::BasicVehicleAssignmentFor<A, Instance, typename A::Input>;

    { AssignmentTraits<A>::name() } -> std::same_as<std::string>;
  };

}

#endif // SCHED_ASSIGNMENT_CONCEPTS_H
