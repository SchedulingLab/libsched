#ifndef SCHED_JOB_SHOP_INSTANCE_H
#define SCHED_JOB_SHOP_INSTANCE_H

#include <cassert>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

#include "BasicInstance.h"

namespace sched::shop {

  struct SCHED_API JobShopInstance : BasicInstance {
    struct Operation {
      MachineId machine;
      Time processing;
    };

    struct Job {
      std::vector<Operation> operations;
    };

    JobShopInstance() = default;

    JobShopInstance(std::size_t machines, std::vector<Job> jobs)
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

    std::vector<MachineId> machines_for_operation(OperationId op) const {
      return { get_job(op.job).operations[op.index].machine };
    }

    Time processing_time(OperationId op, [[maybe_unused]] MachineId machine) const {
      auto &operation = get_job(op.job).operations[op.index];

      if (operation.machine == machine) {
        return operation.processing;
      }

      return TimeMax;
    }

  private:
    const Job& get_job(JobId id) const {
      auto index = sched::to_index(id);
      assert(index < m_jobs.size());
      return m_jobs[index];
    }

    bool is_valid() const noexcept {
      for (auto & job : m_jobs) {
        for (auto & op : job.operations) {
          if (sched::to_index(op.machine) >= m_machines) {
            return false;
          }
        }
      }

      return true;
    }

    std::size_t m_machines;
    std::vector<Job> m_jobs;
  };

}


#endif // SCHED_JOB_SHOP_INSTANCE_H
