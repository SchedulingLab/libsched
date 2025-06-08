// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_INSTANCE_H
#define SCHED_INSTANCE_H

#include <sched/Ids.h>
#include <sched/support/Range.h>
#include <sched/types/InstanceConcepts.h>

namespace sched {

  using MachineRange = Range<MachineId>;
  using JobRange = Range<JobId>;
  using VehicleRange = Range<VehicleId>;

  template<concepts::Instance Instance>
  MachineRange machines(const Instance& instance)
  {
    return MachineRange{ instance.machine_count() };
  }

  template<typename Instance>
  MachineRange devices(const Instance& instance)
  {
    return MachineRange{ instance.device_count() };
  }

  template<concepts::Instance Instance>
  JobRange jobs(const Instance& instance)
  {
    return JobRange{ instance.job_count() };
  }

  template<typename Instance>
  VehicleRange vehicles(const Instance& instance)
  {
    return VehicleRange{ instance.vehicle_count() };
  }

  struct OperationRange {
    JobId job;
    std::size_t size;

    struct Iterator {
      using difference_type = std::ptrdiff_t;
      using value_type = OperationId;
      using pointer = value_type;
      using reference = value_type;
      using iterator_category = std::bidirectional_iterator_tag;

      JobId job;
      std::size_t index;

      void swap(Iterator& other) noexcept
      {
        using std::swap;
        swap(index, other.index);
      }

      reference operator*() noexcept
      {
        return OperationId{ .job = job, .index = index };
      }

      pointer operator->() noexcept
      {
        return OperationId{ .job = job, .index = index };
      }

      Iterator& operator++() noexcept
      {
        ++index;
        return *this;
      }

      Iterator operator++(int) noexcept
      {
        Iterator copy = *this;
        ++index;
        return copy;
      }

      Iterator& operator--() noexcept
      {
        --index;
        return *this;
      }

      constexpr bool operator!=(const Iterator& other) const noexcept
      {
        return index != other.index;
      }

      constexpr bool operator==(const Iterator& other) const noexcept
      {
        return index == other.index;
      }
    };

    constexpr Iterator begin() const noexcept
    {
      return Iterator{ .job = job, .index = 0 };
    }

    constexpr Iterator end() const noexcept
    {
      return Iterator{ .job = job, .index = size };
    }
  };

  template<concepts::Instance Instance>
  OperationRange operations(const Instance& instance, JobId job)
  {
    return OperationRange{ job, instance.operation_count(job) };
  }

}

#endif // SCHED_INSTANCE_H
