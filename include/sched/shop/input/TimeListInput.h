// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_TIME_INPUT_H
#define SCHED_SHOP_TIME_INPUT_H

#include <algorithm>
#include <string>
#include <vector>

#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/meta/Instance.h>
#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>

namespace sched {

  namespace shop {
    using TimeListInput = std::vector<Time>;
  }

  template<>
  struct InputTraits<shop::TimeListInput> {
    static std::string name()
    {
      return "tim";
    }

    template<typename Instance>
    static shop::TimeListInput generate_input(const Instance& instance)
    {
      shop::TimeListInput input;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          input.push_back(1);
        }
      }

      return input;
    }

    template<typename Instance>
    static shop::TimeListInput generate_random(const Instance& instance, Random& random)
    {
      std::size_t count = 0;
      Time max_time = 0;

      for (auto job : sched::jobs(instance)) {
        const auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          const OperationId operation = { job, i };

          if constexpr (Instance::Flexible) {
            const auto machines = instance.machines_for_operation(operation);

            for (auto& machine : machines) {
              const Time processing_time = instance.processing_time(operation, machine);
              max_time = std::max(processing_time, max_time);
            }
          } else {
            const auto machine = instance.assigned_machine_for_operation(operation);
            const Time processing_time = instance.processing_time(operation, machine);
            max_time = std::max(processing_time, max_time);
          }

          ++count;
        }
      }

      shop::TimeListInput input;
      std::uniform_int_distribution<Time> dist(0, max_time);

      for (std::size_t i = 0; i < count; ++i) {
        input.push_back(dist(random));
      }

      return input;
    }

    template<typename Instance>
    static shop::TimeListInput generate_feasible(const Instance& instance, [[maybe_unused]] Random& random)
    {
      return generate_random(instance, random);
    }
  };

}

#endif // SCHED_SHOP_TIME_INPUT_H
