#ifndef SCHED_SHOP_FLOAT_LIST_ASSIGNMENT_H
#define SCHED_SHOP_FLOAT_LIST_ASSIGNMENT_H

#include <cassert>

#include <map>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "FloatListInput.h"

namespace sched::shop {

  struct SCHED_API FloatListAssignment {
    using Input = FloatListInput;

    template<typename Instance>
    std::map<OperationId, MachineId> operator()(const Instance& instance, const FloatListInput& input)
    {
      static_assert(Instance::Flexible, "Instance should be flexible.");
      std::map<OperationId, MachineId> assignment;
      std::size_t input_index = 0;

      for (auto job : sched::jobs(instance)) {
        OperationId operation = {};
        operation.job = job;
        auto operation_count = instance.operation_count(job);

        for (std::size_t index = 0; index < operation_count; ++index) {
          operation.index = index;
          auto available = instance.machines_for_operation(operation);
          auto machine_count = available.size();
          assert(machine_count > 0);
          auto machine_index = static_cast<std::size_t>(input[input_index] * static_cast<double>(machine_count));
          assert(machine_index < machine_count);

          [[maybe_unused]] auto [iterator, inserted] = assignment.insert({ operation, available[machine_index] });
          assert(inserted);

          ++input_index;
        }
      }

      return assignment;
    }
  };

}

#endif // SCHED_SHOP_FLOAT_LIST_ASSIGNMENT_H
