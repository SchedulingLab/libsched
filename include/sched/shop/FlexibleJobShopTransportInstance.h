#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H

#include <cassert>

#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Array2D.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

#include "JobShopData.h"

namespace sched::shop {

  struct SCHED_API FlexibleJobShopTransportInstance {
    static constexpr bool flexible = true;

    FlexibleJobShopTransportInstance() = default;

    FlexibleJobShopTransportInstance(FlexibleJobShopTransportData data)
    : m_data(std::move(data))
    {
      assert(is_valid());
    }

    std::size_t machine_count() const noexcept
    {
      return m_data.machines;
    }

    std::size_t job_count() const noexcept
    {
      return m_data.jobs.size();
    }

    std::size_t operation_count(JobId job) const
    {
      return get_job(job).operations.size();
    }

    std::vector<MachineId> machines_for_operation(OperationId op) const
    {
      std::vector<MachineId> machines;

      for (const auto& choice : get_job(op.job).operations[op.index].choices) {
        machines.push_back(choice.machine);
      }

      return machines;
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

    constexpr std::size_t transportation_count() const noexcept
    {
      return m_data.vehicles;
    }

    Time transportation_time_empty(MachineId origin, MachineId target) const noexcept
    {
      if (origin == NoMachine) {
        return Time{ 0 };
      }

      return m_data.empty(sched::to_index(origin), sched::to_index(target));
    }

    Time transportation_time_loaded(MachineId origin, MachineId target) const noexcept
    {
      if (origin == NoMachine) {
        return Time{ 0 };
      }

      return m_data.loaded(sched::to_index(origin), sched::to_index(target));
    }

  private:
    const FlexibleJobData& get_job(JobId id) const
    {
      auto index = sched::to_index(id);
      assert(index < m_data.jobs.size());
      return m_data.jobs[index];
    }

    bool is_valid() const noexcept
    {
      for (const auto& job : m_data.jobs) {
        for (const auto& op : job.operations) {
          for (const auto& choice : op.choices) {
            if (sched::to_index(choice.machine) >= m_data.machines) {
              return false;
            }
          }
        }
      }

      if (m_data.empty.rows() != m_data.machines || m_data.empty.cols() != m_data.machines) {
        return false;
      }

      if (m_data.loaded.rows() != m_data.machines || m_data.loaded.cols() != m_data.machines) {
        return false;
      }

      return true;
    }

    FlexibleJobShopTransportData m_data = {};
  };

}

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
