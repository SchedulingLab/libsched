#ifndef SCHED_FLEXIBLE_JOBSHOP_WITH_TRANSPORTATION_INSTANCE_H
#define SCHED_FLEXIBLE_JOBSHOP_WITH_TRANSPORTATION_INSTANCE_H

#include <cassert>
#include <limits>
#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

#include "BasicInstance.h"
#include "Transportation.h"

namespace sched::shop {

  struct SCHED_API FlexibleJobShopWithTransportationInstance : BasicInstance {
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

    struct TransportationDesc {
      MachineId origin;
      MachineId target;
      Transportation transportation;
      Time duration;
    };

    FlexibleJobShopWithTransportationInstance() = default;

    FlexibleJobShopWithTransportationInstance(std::size_t machines, std::vector<JobDesc> jobs, std::size_t transportation_resources, std::vector<TransportationDesc> delays)
    : m_machines(machines)
    , m_jobs(std::move(jobs))
    , m_transportation_resources(transportation_resources)
    , m_delays(std::move(delays))
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
      return transportation_time(origin, target, Transportation::Empty);
    }

    Time transportation_time_loaded(MachineId origin, MachineId target) const noexcept {
      return transportation_time(origin, target, Transportation::Loaded);
    }

  private:
    const JobDesc& get_job(JobId id) const {
      auto index = sched::to_index(id);
      assert(index < m_jobs.size());
      return m_jobs[index];
    }

    Time transportation_time(MachineId origin, MachineId target, Transportation transportation) const noexcept {
      if (origin == target) {
        return 0;
      }

      for (auto & delay : m_delays) {
        if (delay.origin == origin && delay.target == target && delay.transportation == transportation) {
          return delay.duration;
        }
      }

      return TimeMax;
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

      for (auto & delay : m_delays) {
        if (sched::to_index(delay.origin) >= m_machines) {
          return false;
        }

        if (sched::to_index(delay.target) >= m_machines) {
          return false;
        }
      }

      return true;
    }

    std::size_t m_machines;
    std::vector<JobDesc> m_jobs;
    std::size_t m_transportation_resources;
    std::vector<TransportationDesc> m_delays;
  };

  using FjstInstance = FlexibleJobShopWithTransportationInstance;

}


#endif // SCHED_FLEXIBLE_JOB_WITH_TRANSPORTATION_SHOP_INSTANCE_H
