// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
#define SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H

#include <cassert>

#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/support/Array2D.h>

#include <sched/shop/instance/JobShopData.h>
#include <sched/shop/Transportation.h>

namespace sched::shop {

  struct SCHED_API FlexibleJobShopTransportInstance {
    static constexpr bool Flexible = true;

    FlexibleJobShopTransportInstance() = default;

    FlexibleJobShopTransportInstance(FlexibleJobShopTransportData data, TransportationMode mode = TransportationMode::None)
    : m_data(std::move(data))
    , m_mode(mode)
    {
      if (mode == TransportationMode::Load || mode == TransportationMode::LoadUnload) {
        FlexibleOperationData load_operation;
        load_operation.choices.push_back({ data.load, 0 });

        for (auto& job : m_data.jobs) {
          job.operations.insert(job.operations.begin(), load_operation);
        }
      }

      if (mode == TransportationMode::Unload || mode == TransportationMode::LoadUnload) {
        FlexibleOperationData unload_operation;
        unload_operation.choices.push_back({ data.unload, 0 });

        for (auto& job : m_data.jobs) {
          job.operations.push_back(unload_operation);
        }
      }

      assert(valid());
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

    std::size_t vehicle_count() const noexcept
    {
      return m_data.vehicles;
    }

    Time transportation_time_empty(MachineId origin, MachineId target) const
    {
      if (origin == NoMachine) {
        return Time{ 0 };
      }

      return m_data.empty(sched::to_index(origin), sched::to_index(target));
    }

    Time transportation_time_loaded(MachineId origin, MachineId target) const
    {
      if (origin == NoMachine) {
        return Time{ 0 };
      }

      return m_data.loaded(sched::to_index(origin), sched::to_index(target));
    }

    std::size_t station_count() const noexcept
    {
      return m_data.stations;
    }

    MachineId load_station() const noexcept
    {
      return m_data.load;
    }

    MachineId unload_station() const noexcept
    {
      return m_data.unload;
    }

    const FlexibleJobShopTransportData& data() const
    {
      return m_data;
    }

    TransportationMode mode() const
    {
      return m_mode;
    }

    std::size_t device_count() const
    {
      return m_data.machines + m_data.stations;
    }

  private:
    const FlexibleJobData& get_job(JobId id) const
    {
      auto index = sched::to_index(id);
      assert(index < m_data.jobs.size());
      return m_data.jobs[index];
    }

    bool valid() const noexcept
    {
      std::size_t device_count = m_data.machines + m_data.stations;

      for (const auto& job : m_data.jobs) {
        for (const auto& op : job.operations) {
          for (const auto& choice : op.choices) {
            if (sched::to_index(choice.machine) >= device_count) {
              return false;
            }
          }
        }
      }

      if (m_data.empty.rows() != device_count || m_data.empty.cols() != device_count) {
        return false;
      }

      if (m_data.loaded.rows() != device_count || m_data.loaded.cols() != device_count) {
        return false;
      }

      return true;
    }

    FlexibleJobShopTransportData m_data = {};
    TransportationMode m_mode = TransportationMode::None;
  };

  SCHED_API FlexibleJobShopTransportInstance import_fjspt_json(const std::filesystem::path& filename, TransportationMode mode = TransportationMode::None);
  SCHED_API void export_fjspt_json(const std::filesystem::path& filename, const FlexibleJobShopTransportInstance& instance);

}

#endif // SCHED_SHOP_FLEXIBLE_JOB_SHOP_TRANSPORT_INSTANCE_H
