// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_INSTANCE_H
#define SCHED_SHOP_JOB_SHOP_INSTANCE_H

#include <cassert>

#include <filesystem>
#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/shop/instance/JobShopData.h>

namespace sched::shop {

  struct SCHED_API JobShopInstance {
    static constexpr bool Flexible = false;

    JobShopInstance() = default;

    JobShopInstance(JobShopData data)
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

    std::size_t device_count() const
    {
      return m_data.machines;
    }

  private:
    const JobData& get_job(JobId id) const
    {
      auto index = sched::to_index(id);
      assert(index < m_data.jobs.size());
      return m_data.jobs[index];
    }

    bool is_valid() const noexcept
    {
      for (const auto& job : m_data.jobs) {
        for (const auto& op : job.operations) {
          if (to_index(op.machine) >= m_data.machines) {
            return false;
          }
        }
      }

      return true;
    }

    JobShopData m_data;
  };

  SCHED_API JobShopInstance import_jsp_txt(const std::filesystem::path& filename);

}

#endif // SCHED_SHOP_JOB_SHOP_INSTANCE_H
