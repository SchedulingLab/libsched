// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_INSTANCE_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_INSTANCE_H

#include <cassert>

#include <filesystem>
#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/shop/Transportation.h>
#include <sched/shop/instance/JobShopTransportData.h>
#include <sched/support/Array2D.h>

namespace sched::shop {

  struct SCHED_API JobShopTransportInstance {
    static constexpr bool Flexible = false;

    JobShopTransportInstance() = default;

    JobShopTransportInstance(JobShopTransportData data, TransportationMode mode = TransportationMode::None)
    : m_data(std::move(data))
    , m_mode(mode)
    {
      if (mode == TransportationMode::Load || mode == TransportationMode::LoadUnload) {
        const OperationData load_operation = { .machine = data.load, .processing = 0 };

        for (JobData& job : m_data.jobs) {
          job.operations.insert(job.operations.begin(), load_operation);
        }
      }

      if (mode == TransportationMode::Unload || mode == TransportationMode::LoadUnload) {
        OperationData unload_operation = { .machine = data.unload, .processing = 0 };

        for (JobData& job : m_data.jobs) {
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

    MachineId assigned_machine_for_operation([[maybe_unused]] OperationId op) const
    {
      return get_job(op.job).operations[op.index].machine;
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
      const OperationData& operation = get_job(op.job).operations[op.index];

      if (operation.machine == machine) {
        return operation.processing;
      }

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

    std::size_t device_count() const
    {
      return m_data.machines + m_data.stations;
    }

    const JobShopTransportData& data() const
    {
      return m_data;
    }

    TransportationMode mode() const
    {
      return m_mode;
    }

  private:
    const JobData& get_job(JobId id) const
    {
      const std::size_t index = sched::to_index(id);
      assert(index < m_data.jobs.size());
      return m_data.jobs[index];
    }

    bool valid() const noexcept
    {
      const std::size_t device_count = m_data.machines + m_data.stations;

      for (const JobData& job : m_data.jobs) {
        for (const OperationData& operation : job.operations) {
          if (sched::to_index(operation.machine) >= device_count) {
            return false;
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

    JobShopTransportData m_data = {};
    TransportationMode m_mode = TransportationMode::None;
  };

  SCHED_API JobShopTransportInstance import_jspt_json(const std::filesystem::path& filename, TransportationMode mode = TransportationMode::None);
  SCHED_API void export_jspt_json(const std::filesystem::path& filename, const JobShopTransportInstance& instance);

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_INSTANCE_H
