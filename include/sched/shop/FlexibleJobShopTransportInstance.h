#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H

#include <cassert>
#include <limits>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

#include "Transportation.h"

namespace sched::shop {

  struct SCHED_API FlexibleJobShopTransportInstance {
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

    FlexibleJobShopTransportInstance() = default;

    FlexibleJobShopTransportInstance(std::size_t machines, std::vector<JobDesc> jobs, std::size_t transportation_resources, std::vector<Time> delays_empty, std::vector<Time> delays_loaded)
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

      assert(sched::to_index(origin) < m_machines);
      assert(sched::to_index(target) < m_machines);
      std::size_t index = sched::to_index(origin) * m_machines + sched::to_index(target);
      assert(index < m_delays_empty.size());
      return m_delays_empty[index];
    }

    Time transportation_time_loaded(MachineId origin, MachineId target) const noexcept {
      if (origin == NoMachine) {
        return Time{0};
      }

      assert(sched::to_index(origin) < m_machines);
      assert(sched::to_index(target) < m_machines);
      std::size_t index = sched::to_index(origin) * m_machines + sched::to_index(target);
      assert(index < m_delays_loaded.size());
      return m_delays_loaded[index];
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

      if (m_delays_empty.size() != m_machines * m_machines) {
        return false;
      }

      if (m_delays_loaded.size() != m_machines * m_machines) {
        return false;
      }

      return true;
    }

    std::size_t m_machines;
    std::vector<JobDesc> m_jobs;
    std::size_t m_transportation_resources;
    std::vector<Time> m_delays_empty;
    std::vector<Time> m_delays_loaded;
  };

}


#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
