#ifndef SCHED_INSTANCE_H
#define SCHED_INSTANCE_H

#include <sched/Ids.h>
#include <sched/support/Range.h>
#include <sched/types/InstanceConcepts.h>

namespace sched {

  using MachineRange = Range<MachineId>;
  using JobRange = Range<JobId>;
  using TransportationRange = Range<TransportationId>;

  template<concepts::Instance Instance>
  MachineRange machines(const Instance& instance)
  {
    return MachineRange{ instance.machine_count() };
  }

  template<concepts::Instance Instance>
  JobRange jobs(const Instance& instance)
  {
    return JobRange{ instance.job_count() };
  }

  template<typename Instance>
  TransportationRange transportations(const Instance& instance)
  {
    return TransportationRange{ instance.transportation_count() };
  }

}

#endif // SCHED_INSTANCE_H
