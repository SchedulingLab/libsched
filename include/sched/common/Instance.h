#ifndef SCHED_INSTANCE_H
#define SCHED_INSTANCE_H

#include "Ids.h"
#include "Range.h"

namespace sched {

  using MachineRange = Range<MachineId>;
  using JobRange = Range<JobId>;
  using TransportationRange = Range<TransportationId>;

  template<typename Instance>
  MachineRange machines(const Instance& instance) {
    return MachineRange{ instance.machine_count() };
  }

  template<typename Instance>
  JobRange jobs(const Instance& instance) {
    return JobRange{ instance.job_count() };
  }

  template<typename Instance>
  TransportationRange transportations(const Instance& instance) {
    return TransportationRange{ instance.transportation_count() };
  }

}

#endif // SCHED_INSTANCE_H
