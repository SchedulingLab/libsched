// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_IDS_H
#define SCHED_IDS_H

#include <cstdint>

#include <limits>
#include <string>
#include <utility>

#include <sched/Api.h>

namespace sched {

  namespace details {

    constexpr uint32_t NoId = std::numeric_limits<uint32_t>::max();
    constexpr uint32_t AnyId = NoId - 1;

  }

  /*
   * MachineId
   */

  enum MachineId : uint32_t {
  };

  constexpr MachineId to_machine(std::size_t i)
  {
    return MachineId{ static_cast<uint32_t>(i) };
  }

  constexpr std::size_t to_index(MachineId id)
  {
    return static_cast<std::size_t>(id);
  }

  constexpr MachineId AnyMachine = MachineId{ details::AnyId };
  constexpr MachineId NoMachine = MachineId{ details::NoId };

  inline std::string to_string(MachineId id)
  {
    return std::to_string(std::to_underlying(id));
  }

  /*
   * JobId
   */

  enum JobId : uint32_t {
  };

  constexpr JobId to_job(std::size_t i)
  {
    return JobId{ static_cast<uint32_t>(i) };
  }

  constexpr std::size_t to_index(JobId id)
  {
    return static_cast<std::size_t>(id);
  }

  constexpr JobId AnyJob = JobId{ details::AnyId };
  constexpr JobId NoJob = JobId{ details::NoId };

  inline std::string to_string(JobId id)
  {
    return std::to_string(std::to_underlying(id));
  }

  /*
   * OperationId
   */

  struct SCHED_API OperationId {
    JobId job;
    uint32_t index;
  };

  constexpr OperationId to_operation(JobId job, std::size_t index)
  {
    return { .job = job, .index = static_cast<uint32_t>(index) };
  }

  constexpr bool operator<(const OperationId& lhs, const OperationId& rhs)
  {
    if (lhs.job == rhs.job) {
      return lhs.index < rhs.index;
    }

    return lhs.job < rhs.job;
  }

  constexpr bool operator==(const OperationId& lhs, const OperationId& rhs)
  {
    return lhs.job == rhs.job && lhs.index == rhs.index;
  }

  constexpr bool operator!=(const OperationId& lhs, const OperationId& rhs)
  {
    return !(lhs == rhs);
  }

  inline std::string to_string(OperationId id)
  {
    return to_string(id.job) + '_' + std::to_string(id.index);
  }

  /*
   * VehicleId
   */

  enum VehicleId : uint32_t {
  };

  constexpr VehicleId to_vehicle(std::size_t i)
  {
    return VehicleId{ static_cast<uint32_t>(i) };
  }

  constexpr std::size_t to_index(VehicleId id)
  {
    return static_cast<std::size_t>(id);
  }

  constexpr VehicleId AnyVehicle = VehicleId{ details::AnyId };
  constexpr VehicleId NoVehicle = VehicleId{ details::NoId };

  inline std::string to_string(VehicleId id)
  {
    return std::to_string(std::to_underlying(id));
  }

  /*
   * literals for various ids
   */

  namespace literals {

    constexpr MachineId operator""_m(unsigned long long val)
    {
      return static_cast<MachineId>(val);
    }

    constexpr JobId operator""_j(unsigned long long val)
    {
      return static_cast<JobId>(val);
    }

    constexpr VehicleId operator""_v(unsigned long long val)
    {
      return static_cast<VehicleId>(val);
    }

  }

}

#endif // SCHED_IDS_H
