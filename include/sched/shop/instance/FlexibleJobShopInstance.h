// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLEXIBLE_JOBSHOP_INSTANCE_H
#define SCHED_SHOP_FLEXIBLE_JOBSHOP_INSTANCE_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>

namespace sched::shop {

  struct SCHED_API FlexibleJobShopInstance {
    static constexpr bool Flexible = true;

    struct FlexibleOperationDesc {
      MachineId machine;
      Time processing;
    };

    struct OperationDesc {
      std::vector<FlexibleOperationDesc> choices;
    };

    struct JobDesc {
      std::vector<OperationDesc> operations;
    };

    FlexibleJobShopInstance() = default;

    FlexibleJobShopInstance(std::size_t machines, std::vector<JobDesc> jobs)
    : m_machines(machines)
    , m_jobs(std::move(jobs))
    {
      assert(is_valid());
    }

    std::size_t machine_count() const noexcept
    {
      return m_machines;
    }

    std::size_t job_count() const noexcept
    {
      return m_jobs.size();
    }

    std::size_t operation_count(JobId job) const
    {
      return get_job(job).operations.size();
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    constexpr Time release_date([[maybe_unused]] JobId job) const noexcept
    {
      return 0;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    constexpr Time due_date([[maybe_unused]] JobId job) const noexcept
    {
      return TimeMax;
    }

    std::vector<MachineId> machines_for_operation(OperationId op) const
    {
      std::vector<MachineId> machines;

      for (const auto& choice : get_job(op.job).operations[op.index].choices) {
        machines.push_back(choice.machine);
      }

      return machines;
    }

    Time processing_time(OperationId op, [[maybe_unused]] MachineId machine) const
    {
      for (const auto& choice : get_job(op.job).operations[op.index].choices) {
        if (choice.machine == machine) {
          return choice.processing;
        }
      }

      assert(false);
      return TimeMax;
    }

  private:
    const JobDesc& get_job(JobId id) const
    {
      auto index = sched::to_index(id);
      assert(index < m_jobs.size());
      return m_jobs[index];
    }

    bool is_valid() const noexcept
    {
      for (const auto& job : m_jobs) {
        for (const auto& op : job.operations) {
          for (const auto& choice : op.choices) {
            if (sched::to_index(choice.machine) >= m_machines) {
              return false;
            }
          }
        }
      }

      return true;
    }

    std::size_t m_machines = 0;
    std::vector<JobDesc> m_jobs;
  };

}

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_INSTANCE_H
