#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H

#include <cassert>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Array2D.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

#include "Transportation.h"

namespace sched::shop {

  struct SCHED_API FlexibleJobShopTransportInstance {
    static constexpr bool flexible = true;

    struct OperationDesc {
      MachineId machine;
      Time processing;
    };

    struct FlexibleOperationDesc {
      std::vector<OperationDesc> choices;
    };

    struct JobDesc {
      std::vector<FlexibleOperationDesc> operations;
    };

    FlexibleJobShopTransportInstance() = default;

    FlexibleJobShopTransportInstance(std::size_t machines, std::vector<JobDesc> jobs, std::size_t transportation_resources, Array2D<Time> delays_empty, Array2D<Time> delays_loaded)
    : m_machines(machines)
    , m_jobs(std::move(jobs))
    , m_transportation_resources(transportation_resources)
    , m_delays_empty(std::move(delays_empty))
    , m_delays_loaded(std::move(delays_loaded))
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
      std::vector<MachineId> machines;

      for (auto& choice : get_job(op.job).operations[op.index].choices) {
        machines.push_back(choice.machine);
      }

      return machines;
    }

    constexpr Time release_date([[maybe_unused]] JobId job) const noexcept {
      return 0;
    }

    constexpr Time due_date([[maybe_unused]] JobId job) const noexcept {
      return TimeMax;
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

    constexpr std::size_t transportation_count() const noexcept {
      return m_transportation_resources;
    }

    Time transportation_time_empty(MachineId origin, MachineId target) const noexcept {
      if (origin == NoMachine) {
        return Time{0};
      }

      return m_delays_empty(sched::to_index(origin), sched::to_index(target));
    }

    Time transportation_time_loaded(MachineId origin, MachineId target) const noexcept {
      if (origin == NoMachine) {
        return Time{0};
      }

      return m_delays_loaded(sched::to_index(origin), sched::to_index(target));
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

      if (m_delays_empty.rows() != m_machines || m_delays_empty.cols() != m_machines) {
        return false;
      }

      if (m_delays_loaded.rows() != m_machines || m_delays_loaded.cols() != m_machines) {
        return false;
      }

      return true;
    }

    std::size_t m_machines = 0;
    std::vector<JobDesc> m_jobs;
    std::size_t m_transportation_resources;
    Array2D<Time> m_delays_empty;
    Array2D<Time> m_delays_loaded;
  };

}


#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
