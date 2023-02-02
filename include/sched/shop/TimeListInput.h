#ifndef SCHED_SHOP_TIME_INPUT_H
#define SCHED_SHOP_TIME_INPUT_H

#include <string>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/Instance.h>
#include <sched/common/Random.h>
#include <sched/common/Time.h>

#include "InputTraits.h"

namespace sched::shop {

  using TimeListInput = std::vector<Time>;

  template<>
  struct InputTraits<TimeListInput> {
    static std::string name() {
      return "tim";
    }

    template<typename Instance>
    static TimeListInput generate_input(const Instance& instance) {
      TimeListInput input;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          input.push_back(1);
        }
      }

      return input;
    }

    template<typename Instance>
    static TimeListInput generate_random(const Instance& instance, Random& random) {
      std::size_t count = 0;
      Time max_time = 0;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          const OperationId operation = { job, i };

          if constexpr (Instance::flexible) {
            const auto machines = instance.machines_for_operation(operation);

            for (auto & machine : machines) {
              Time processing_time = instance.processing_time(operation, machine);

              if (processing_time > max_time) {
                max_time = processing_time;
              }
            }
          } else {
            const auto machine = instance.assigned_machine_for_operation(operation);
            Time processing_time = instance.processing_time(operation, machine);

            if (processing_time > max_time) {
              max_time = processing_time;
            }
          }

          ++count;
        }
      }

      TimeListInput input;
      std::uniform_int_distribution<Time> dist(0, max_time);

      for (std::size_t i = 0; i < count; ++i) {
        input.push_back(dist(random));
      }

      return input;
    }

    template<typename Instance>
    static TimeListInput generate_feasible(const Instance& instance, [[maybe_unused]] Random& random) {
      return generate_random(instance, random);
    }

  };

}

#endif // SCHED_SHOP_TIME_INPUT_H
