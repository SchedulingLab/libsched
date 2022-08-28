#ifndef SCHED_FLEXIBLE_JOBSHOP_INSTANCE_H
#define SCHED_FLEXIBLE_JOBSHOP_INSTANCE_H

#include <cassert>
#include <limits>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

namespace sched::shop {

  struct SCHED_API FlexibleJobShopInstance {
    static constexpr bool flexible = true;

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

    std::size_t machine_count() const noexcept {
      return m_machines;
    }

    std::size_t job_count() const noexcept {
      return m_jobs.size();
    }

    std::size_t operation_count(JobId job) const {
      return get_job(job).operations.size();
    }

    constexpr Time release_date([[maybe_unused]] JobId job) const noexcept {
      return 0;
    }

    constexpr Time due_date([[maybe_unused]] JobId job) const noexcept {
      return TimeMax;
    }

    std::vector<MachineId> machines_for_operation(OperationId op) const {
      std::vector<MachineId> machines;

      for (auto& choice : get_job(op.job).operations[op.index].choices) {
        machines.push_back(choice.machine);
      }

      return machines;
    }

    Time processing_time(OperationId op, [[maybe_unused]] MachineId machine) const {
      for (auto & choice : get_job(op.job).operations[op.index].choices) {
        if (choice.machine == machine) {
          return choice.processing;
        }
      }

      assert(false);
      return TimeMax;
    }

  private:
    const JobDesc& get_job(JobId id) const {
      auto index = sched::to_index(id);
      assert(index < m_jobs.size());
      return m_jobs[index];
    }

    bool is_valid() const noexcept {
      for (auto & job : m_jobs) {
        for (auto & op : job.operations) {
          for (auto & choice : op.choices) {
            if (sched::to_index(choice.machine) >= m_machines) {
              return false;
            }
          }
        }
      }

      return true;
    }

    std::size_t m_machines;
    std::vector<JobDesc> m_jobs;
  };

}


#endif // SCHED_FLEXIBLE_JOB_SHOP_INSTANCE_H
