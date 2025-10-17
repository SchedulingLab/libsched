// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLEXIBLE_JOBSHOP_INSTANCE_H
#define SCHED_SHOP_FLEXIBLE_JOBSHOP_INSTANCE_H

#include <cassert>

#include <filesystem>
#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/shop/instance/FlexibleJobShopData.h>

namespace sched::shop {

  struct SCHED_API FlexibleJobShopInstance {
    static constexpr bool Flexible = true;

    FlexibleJobShopInstance() = default;

    FlexibleJobShopInstance(FlexibleJobShopData data)
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

    std::size_t device_count() const
    {
      return m_data.machines;
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
        for (const auto& operation : job.operations) {
          for (const auto& choice : operation.choices) {
            if (to_index(choice.machine) >= m_data.machines) {
              return false;
            }
          }
        }
      }

      return true;
    }

    FlexibleJobShopData m_data;
  };

  SCHED_API FlexibleJobShopInstance import_fjsp_txt(const std::filesystem::path& filename);

}

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_INSTANCE_H
