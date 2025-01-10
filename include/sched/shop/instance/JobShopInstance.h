#ifndef SCHED_JOB_SHOP_INSTANCE_H
#define SCHED_JOB_SHOP_INSTANCE_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>

namespace sched::shop {

  struct SCHED_API JobShopInstance {
    static constexpr bool Flexible = false;

    struct OperationDesc {
      MachineId machine;
      Time processing;
    };

    struct JobDesc {
      std::vector<OperationDesc> operations;
    };

    JobShopInstance() = default;

    JobShopInstance(std::size_t machines, std::vector<JobDesc> jobs)
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

    MachineId assigned_machine_for_operation([[maybe_unused]] OperationId op) const
    {
      return get_job(op.job).operations[op.index].machine;
    }

    Time processing_time(OperationId op, [[maybe_unused]] MachineId machine) const
    {
      const auto& operation = get_job(op.job).operations[op.index];

      if (operation.machine == machine) {
        return operation.processing;
      }

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
          if (sched::to_index(op.machine) >= m_machines) {
            return false;
          }
        }
      }

      return true;
    }

    std::size_t m_machines = 0;
    std::vector<JobDesc> m_jobs;
  };

}

#endif // SCHED_JOB_SHOP_INSTANCE_H
