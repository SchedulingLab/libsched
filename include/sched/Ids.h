// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_IDS_H
#define SCHED_IDS_H

#include <cstddef>

#include <limits>

#include <sched/Api.h>

namespace sched {

  namespace details {

    constexpr std::size_t NoId = std::numeric_limits<std::size_t>::max();
    constexpr std::size_t AnyId = NoId - 1;

  }

  /*
   * MachineId
   */

  enum MachineId : std::size_t {
  };

  constexpr MachineId machine(std::size_t i)
  {
    return MachineId{ i };
  }

  constexpr std::size_t to_index(MachineId id)
  {
    return static_cast<std::size_t>(id);
  }

  constexpr MachineId AnyMachine = MachineId{ details::AnyId };
  constexpr MachineId NoMachine = MachineId{ details::NoId };

  /*
   * JobId
   */

  enum JobId : std::size_t {
  };

  constexpr JobId job(std::size_t i)
  {
    return JobId{ i };
  }

  constexpr std::size_t to_index(JobId id)
  {
    return static_cast<std::size_t>(id);
  }

  constexpr JobId AnyJob = JobId{ details::AnyId };
  constexpr JobId NoJob = JobId{ details::NoId };

  /*
   * OperationId
   */

  struct SCHED_API OperationId {
    JobId job;
    std::size_t index;
  };

  constexpr OperationId operation(JobId job, std::size_t index)
  {
    return { .job = job, .index = index };
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

  /*
   * TransportationId
   */

  enum TransportationId : std::size_t {
  };

  constexpr std::size_t to_index(TransportationId id)
  {
    return static_cast<std::size_t>(id);
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

    constexpr TransportationId operator""_t(unsigned long long val)
    {
      return static_cast<TransportationId>(val);
    }

  }

}

#endif // SCHED_IDS_H
