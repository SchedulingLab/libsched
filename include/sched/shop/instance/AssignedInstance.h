// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_ASSIGNED_INSTANCE_H
#define SCHED_SHOP_ASSIGNED_INSTANCE_H

#include <cassert>

#include <map>
#include <utility>

#include <sched/Ids.h>
#include <sched/Time.h>

namespace sched::shop {

  template<typename Instance>
  struct AssignedInstance {
    static constexpr bool Flexible = false;

    AssignedInstance() = default;

    AssignedInstance(const Instance& instance)
    : m_instance(&instance)
    {
    }

    std::size_t machine_count() const noexcept(noexcept(std::declval<Instance>().machine_count()))
    {
      return m_instance->machine_count();
    }

    std::size_t job_count() const noexcept(noexcept(std::declval<Instance>().job_count()))
    {
      return m_instance->job_count();
    }

    std::size_t operation_count(JobId job) const noexcept(noexcept(std::declval<Instance>().operation_count(JobId())))
    {
      return m_instance->operation_count(job);
    }

    Time release_date(JobId job) const noexcept(noexcept(std::declval<Instance>().release_date(JobId())))
    {
      return m_instance->release_date(job);
    }

    Time due_date(JobId job) const noexcept(noexcept(std::declval<Instance>().due_date(JobId())))
    {
      return m_instance->due_date(job);
    }

    void set_assignment(std::map<OperationId, MachineId> assignment)
    {
      m_assignment = std::move(assignment);
    }

    MachineId assigned_machine_for_operation(OperationId op) const
    {
      assert(!m_assignment.empty());

      if (auto it = m_assignment.find(op); it != m_assignment.end()) {
        return it->second;
      }

      return NoMachine;
    }

    constexpr Time processing_time(OperationId op, MachineId machine) const noexcept(noexcept(std::declval<Instance>().processing_time(OperationId(), MachineId())))
    {
      return m_instance->processing_time(op, machine);
    }

    constexpr std::size_t vehicle_count() const noexcept(noexcept(std::declval<Instance>().vehicle_count()))
    {
      return m_instance->vehicle_count();
    }

    constexpr Time transportation_time_empty(MachineId from, MachineId to) const noexcept(noexcept(std::declval<Instance>().transportation_time_empty(MachineId(), MachineId())))
    {
      return m_instance->transportation_time_empty(from, to);
    }

    constexpr Time transportation_time_loaded(MachineId from, MachineId to) const noexcept(noexcept(std::declval<Instance>().transportation_time_loaded(MachineId(), MachineId())))
    {
      return m_instance->transportation_time_loaded(from, to);
    }

    std::size_t station_count() const noexcept
    {
      return m_instance->station_count();
    }

    MachineId load_station() const noexcept
    {
      return m_instance->load_station();
    }

    MachineId unload_station() const noexcept
    {
      return m_instance->unload_station();
    }

    std::size_t device_count() const
    {
      return m_instance->device_count();
    }

  private:
    const Instance* m_instance = nullptr;
    std::map<OperationId, MachineId> m_assignment;
  };

}

#endif // SCHED_SHOP_ASSIGNED_INSTANCE_H
