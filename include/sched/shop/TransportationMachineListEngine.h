#ifndef SCHED_SHOP_TRANSPORTATION_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_TRANSPORTATION_MACHINE_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "FloatListInput.h"
#include "JobShopTransportSchedule.h"
#include "JobShopTransportStates.h"
#include "SplitInput.h"

namespace sched::shop {

  template<typename Comparator, typename TransportationAssignment>
  struct TransportationMachineListEngine {
    using Input = TransportSplitInput<FloatListInput, typename TransportationAssignment::Input>;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const Input& input)
    {
      auto transportation = transportation_assignment(instance, input.transport);
      std::size_t transportation_index = 0;

      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      std::vector<std::vector<std::tuple<OperationId, double>>> machine_assignment = compute_machine_assignment(instance, input);

      for (;;) {
        assert(transportation_index < transportation.size());

        std::vector<JobShopTask> tasks;
        std::vector<JobShopTransportTaskPacket> packets;

        // try to find schedulable operations
        std::size_t finished = 0;

        for (auto machine : sched::machines(instance)) {
          auto& machine_state = states.machines[to_index(machine)];

          auto machine_finished = [&]() {
            if (machine_state.index == machine_assignment[to_index(machine)].size()) {
              // there is no more operation to schedule on this machine
              ++finished;
              return true;
            }

            return false;
          };

          if (machine_finished()) {
            continue;
          }

          for (;;) {
            // check if the next operation is schedulable
            auto& operation = std::get<OperationId>(machine_assignment[to_index(machine)][machine_state.index]);
            auto& job_state = states.jobs[to_index(operation.job)];

            if (operation.index < job_state.operation) {
              // this operation has already been scheduled on another machine
              ++machine_state.index;

              if (machine_finished()) {
                break;
              }

              continue;
            }

            if (operation.index == job_state.operation) {
              // we found one

              if (operation.index == 0) {
                tasks.push_back(states.create_task(operation, machine));
              } else {
                packets.push_back(states.create_packet(operation, machine, transportation[transportation_index]));
              }
            }
          }
        }

        if (finished == instance.machine_count()) {
          break;
        }

        if (!states.choose_and_update(tasks, packets, schedule, comparator)) {
          return std::nullopt;
        }

        transportation_index++;
      }

      return schedule;
    }

    template<typename Instance>
    std::vector<std::vector<std::tuple<OperationId, double>>> compute_machine_assignment(const Instance& instance, const FloatListInput& input)
    {
      std::vector<std::vector<std::tuple<OperationId, double>>> machine_assignment;

      std::size_t index = 0;

      for (auto job : sched::jobs(instance)) {
        auto operations = instance.operation_count(job);

        for (std::size_t i = 0; i < operations; ++i) {
          auto input_element = input[index++];
          OperationId operation = { job, i };

          if constexpr (Instance::flexible) {
            const auto available = instance.machines_for_operation(operation);
            assert(!available.empty());

            for (auto machine : available) {
              assert(to_index(machine) < machine_assignment.size());
              machine_assignment[to_index(machine)].emplace_back(operation, input_element);
            }
          } else { // !flexible
            MachineId machine = instance.assigned_machine_for_operation(operation);
            assert(to_index(machine) < machine_assignment.size());
            machine_assignment[to_index(machine)].emplace_back(operation, input[index++]);
          }
        }
      }

      assert(index == input.size());

      std::for_each(machine_assignment.begin(), machine_assignment.end(), [](const std::vector<std::tuple<OperationId, double>>& machine) {
        std::sort(machine.begin(), machine.end(), [](const std::tuple<OperationId, double>& lhs, const std::tuple<OperationId, double>& rhs) {
          return std::get<double>(lhs) < std::get<double>(rhs);
        });
      });

      return machine_assignment;
    }
  };

}

#endif // SCHED_SHOP_TRANSPORTATION_MACHINE_LIST_ENGINE_H
