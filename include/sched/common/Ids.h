#ifndef SCHED_IDS_H
#define SCHED_IDS_H

#include <cstddef>

#include "RawId.h"

namespace sched {

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

  constexpr MachineId AnyMachine = MachineId{ RawAnyId };
  constexpr MachineId NoMachine = MachineId{ RawNoId };

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

  constexpr JobId AnyJob = JobId{ RawAnyId };
  constexpr JobId NoJob = JobId{ RawNoId };

  /*
   * OperationId
   */

  struct OperationId {
    JobId job;
    std::size_t index;
  };

  constexpr OperationId operation(JobId job, std::size_t index)
  {
    return { job, index };
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

    constexpr MachineId operator"" _m(unsigned long long val)
    {
      return static_cast<MachineId>(val);
    }

    constexpr JobId operator"" _j(unsigned long long val)
    {
      return static_cast<JobId>(val);
    }

    constexpr TransportationId operator"" _t(unsigned long long val)
    {
      return static_cast<TransportationId>(val);
    }

  }

}

#endif // SCHED_IDS_H
