// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_SCHEDULING_INSTANCE_H
#define SCHED_SHOP_HOIST_SCHEDULING_INSTANCE_H

#include <cassert>

#include <filesystem>
#include <vector>

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>
#include <sched/shop/instance/HoistSchedulingData.h>

namespace sched::shop {

  class SCHED_API HoistSchedulingInstance {
  public:
    static constexpr bool Flexible = false;

    HoistSchedulingInstance() = default;

    HoistSchedulingInstance(HoistSchedulingData data)
    : m_data(std::move(data))
    {
      assert(m_data.empty.rows() == m_data.machines && m_data.empty.cols() == m_data.machines);
    }

    std::size_t machine_count() const noexcept {
      return m_data.machines;
    }

    std::size_t job_count() const noexcept {
      return 1;
    }

    std::size_t operation_count([[maybe_unused]] sched::JobId job) const {
      return m_data.operations.size();
    }

    sched::MachineId assigned_machine_for_operation([[maybe_unused]] sched::OperationId op) const {
      assert(op.index < m_data.operations.size());
      return machine(op.index);
    }

    sched::TimeWindow processing_time(sched::OperationId op, [[maybe_unused]] sched::MachineId machine) const {
      assert(op.index < m_data.operations.size());
      assert(sched::machine(op.index) == machine);
      return m_data.operations[op.index].processing;
    }

    constexpr std::size_t vehicle_count() const noexcept {
      return m_data.vehicles;
    }

    sched::Time transportation_time_empty(sched::MachineId origin, sched::MachineId target) const noexcept {
      return m_data.empty(sched::to_index(origin), sched::to_index(target));
    }

    sched::Time transportation_time_loaded(sched::MachineId origin, sched::MachineId target) const noexcept {
      assert(to_index(origin) + 1 == to_index(target));
      return m_data.operations[to_index(origin)].transport;
    }

  private:
    HoistSchedulingData m_data;
  };

  SCHED_API HoistSchedulingInstance load_hoist_scheduling_instance(const std::filesystem::path& filename);

}

#endif // SCHED_SHOP_HOIST_SCHEDULING_INSTANCE_H
