#ifndef SCHED_SHOP_RANDOM_LIST_ASSIGNMENT_H
#define SCHED_SHOP_RANDOM_LIST_ASSIGNMENT_H

#include <cassert>
#include <map>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "RandomListInput.h"

namespace sched::shop {

  struct SCHED_API RandomListAssignment {
    using Input = RandomListInput;

    template<typename Instance>
    std::map<OperationId, MachineId> operator()(const Instance& instance, const RandomListInput& input) {
      static_assert(Instance::flexible, "Instance should be flexible.");
      std::map<OperationId, MachineId> assignment;
      std::size_t input_index = 0;

      for (auto job : sched::jobs(instance)) {
        OperationId operation;
        operation.job = job;
        auto operation_count = instance.operation_count(job);

        for (std::size_t index = 0; index < operation_count; ++index) {
          operation.index = index;
          auto available = instance.machines_for_operation(operation);
          std::size_t machine_count = available.size();
          assert(machine_count > 0);
          std::size_t machine_index = input[input_index] * machine_count;
          assert(machine_index < machine_count);

          [[maybe_unused]] auto [ iterator, inserted ] = assignment.insert({ operation, available[machine_index] });
          assert(inserted);

          ++input_index;
        }
      }

      return assignment;
    }

  };

}

#endif // SCHED_SHOP_RANDOM_LIST_ASSIGNMENT_H
