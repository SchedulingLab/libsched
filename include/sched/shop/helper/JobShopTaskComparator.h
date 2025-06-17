// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H
#define SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H

#include <algorithm>
#include <string>

#include <sched/Api.h>
#include <sched/meta/Instance.h>
#include <sched/shop/schedule/JobShopSchedule.h>

namespace sched::shop {

  struct SCHED_API JobShopTaskEarliestStartingTime {

    template<typename Instance>
    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs, [[maybe_unused]] const Instance& instance) const
    {
      return lhs.start < rhs.start;
    }

    static std::string name()
    {
      return "est";
    }
  };

  struct SCHED_API JobShopTaskLatestStartingTime {

    template<typename Instance>
    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs, [[maybe_unused]] const Instance& instance) const
    {
      return lhs.start > rhs.start;
    }

    static std::string name()
    {
      return "lst";
    }
  };

  struct SCHED_API JobShopTaskEarliestFinishTime {

    template<typename Instance>
    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs, [[maybe_unused]] const Instance& instance) const
    {
      return lhs.completion < rhs.completion;
    }

    static std::string name()
    {
      return "eft";
    }
  };

  struct SCHED_API JobShopTaskLatestFinishTime {

    template<typename Instance>
    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs, [[maybe_unused]] const Instance& instance) const
    {
      return lhs.completion > rhs.completion;
    }

    static std::string name()
    {
      return "lft";
    }
  };

  struct SCHED_API JobShopTaskShortestProcessingTime {

    template<typename Instance>
    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs, [[maybe_unused]] const Instance& instance) const
    {
      return (lhs.completion - lhs.start) < (rhs.completion - rhs.start);
    }

    static std::string name()
    {
      return "spt";
    }
  };

  struct SCHED_API JobShopTaskLargestProcessingTime {

    template<typename Instance>
    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs, [[maybe_unused]] const Instance& instance) const
    {
      return (lhs.completion - lhs.start) > (rhs.completion - rhs.start);
    }

    static std::string name()
    {
      return "lpt";
    }
  };

  namespace details {

    template<typename Instance>
    Time remaining_work(OperationId starting_operation, const Instance& instance)
    {
      Time work = 0;

      for (const OperationId operation : operations(instance, starting_operation.job)) {
        if (operation.index < starting_operation.index) {
          continue;
        }

        if constexpr (Instance::Flexible) {
          std::vector<MachineId> machines = instance.machines_for_operation(operation);

          Time min_time = TimeMax;

          for (const MachineId machine : machines) {
            Time time = instance.processing_time(operation, machine);
            min_time = std::min(time, min_time);
          }

          work += min_time;
        } else {
          const MachineId machine = instance.assigned_machine_for_operation(operation);
          work += instance.processing_time(operation, machine);
        }
      }

      return work;
    }

  };

  struct SCHED_API JobShopTaskMostWorkRemaining {

    template<typename Instance>
    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs, const Instance& instance) const
    {
      return details::remaining_work(lhs, instance) > details::remaining_work(rhs, instance);
    }

    static std::string name()
    {
      return "mwkr";
    }

  };



  template<typename Comparator, typename Instance>
  struct JobShopTaskComparatorAdaptor {
    JobShopTaskComparatorAdaptor(const Instance* instance)
    : instance(instance)
    {
    }

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const
    {
      return comparator(lhs, rhs, *instance);
    }

    Comparator comparator;
    const Instance* instance = nullptr;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H
