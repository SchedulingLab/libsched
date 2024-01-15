#ifndef SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H
#define SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H

#include <cassert>

#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include <sched/common/Ids.h>
#include <sched/common/Instance.h>

#include "JobShopTaskComparator.h"
#include "JobShopTransportSchedule.h"
#include "JobShopTransportStates.h"
#include "RandomListInput.h"

namespace sched::shop {

  template<typename Comparator>
  struct GeneralMachineListEngine {
    using Input = RandomListInput;

    template<typename Instance>
    std::optional<JobShopTransportSchedule> operator()(const Instance& instance, const RandomListInput& input) {
      JobShopTransportStates<Instance> states(instance);
      JobShopTransportSchedule schedule;
      Comparator comparator;

      std::vector<std::vector<std::tuple<OperationId, double>>> machine_assignment;

      if constexpr (Instance::flexible) {
        std::vector<std::vector<std::tuple<OperationId, double>>> machine_assignment;

        std::size_t index = 0;

        for (auto job : sched::jobs(instance)) {
          auto operations = instance.operation_count(job);

          for (std::size_t i = 0; i < operations; ++i) {
            auto input_element = input[index++];
            OperationId operation = { job, i };

            const auto available = instance.machines_for_operation(operation);
            assert(!available.empty());

            for (auto machine : available) {
              assert(to_index(machine) < machine_assignment.size());
              machine_assignment[to_index(machine)].emplace_back(operation, input_element);
            }
          }
        }

        assert(index == input.size());
      } else { // !flexible

        std::size_t index = 0;

        for (auto job : sched::jobs(instance)) {
          auto operations = instance.operation_count(job);

          for (std::size_t i = 0; i < operations; ++i) {
            OperationId operation = { job, i };
            MachineId machine = instance.assigned_machine_for_operation(operation);
            assert(to_index(machine) < machine_assignment.size());
            machine_assignment[to_index(machine)].emplace_back(operation, input[index++]);
          }
        }

        assert(index == input.size());
      }

      for (auto& machine : machine_assignment) {
        std::sort(machine.begin(), machine.end(), [](const std::tuple<OperationId, double>& lhs, const std::tuple<OperationId, double>& rhs) {
          return std::get<double>(lhs) < std::get<double>(rhs);
        });
      }

      for (;;) {
        std::vector<JobShopTask> tasks;
        std::vector<JobShopTransportTaskPacket> packets;

        // try to find schedulable operations
        std::size_t finished = 0;

        for (auto machine : sched::machines(instance)) {
          auto& machine_state = states.machines[to_index(machine)];

          if (machine_state.index == machine_assignment[to_index(machine)].size()) {
            // there is no more operation to schedule on this machine
            ++finished;
            continue;
          }

          for (;;) {
            // check if the next operation is schedulable
            auto& operation = std::get<OperationId>(machine_assignment[to_index(machine)][machine_state.index]);
            auto& job_state = states.jobs[to_index(operation.job)];

            if (operation.index < job_state.operation) {
              // this operation has already been scheduled on another machine
              ++machine_state.index;

              if (machine_state.index == machine_assignment[to_index(machine)].size()) {
                ++finished;
                break;
              }

              continue;
            }

            if (operation.index == job_state.operation) {
              // we found one

              if (operation.index == 0) {
                tasks.push_back(states.create_task(operation, machine));
              } else {
                for (auto transportation : sched::transportations(instance)) {
                  packets.push_back(states.create_packet(operation, machine, transportation));
                }
              }
            }
          }
        }

        if (finished == instance.machine_count()) {
          break;
        }

        if (!tasks.empty()) {
          auto task = *std::min_element(tasks.begin(), tasks.end(), [comparator](const JobShopTask& lhs, const JobShopTask& rhs) {
            return comparator(lhs, rhs);
          });

          states.update_schedule(task, schedule);
        } else if (!packets.empty()) {
          auto packet = *std::min_element(packets.begin(), packets.end(), [comparator](const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
            return comparator(lhs.task, rhs.task);
          });

          states.update_schedule(packet, schedule);
        } else {
          return std::nullopt;
        }
      }

      return schedule;
    }

  };

  using GeneralMachineListEngineEST = GeneralMachineListEngine<JobShopTaskEarliestStartingTime>;
  using GeneralMachineListEngineLST = GeneralMachineListEngine<JobShopTaskLatestStartingTime>;
  using GeneralMachineListEngineEFT = GeneralMachineListEngine<JobShopTaskEarliestFinishTime>;
  using GeneralMachineListEngineLFT = GeneralMachineListEngine<JobShopTaskLatestFinishTime>;
  using GeneralMachineListEngineSPT = GeneralMachineListEngine<JobShopTaskShortestProcessingTime>;
  using GeneralMachineListEngineLPT = GeneralMachineListEngine<JobShopTaskLargestProcessingTime>;

}

#endif // SCHED_SHOP_GENERAL_MACHINE_LIST_ENGINE_H
