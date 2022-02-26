#ifndef SCHED_PARA_SCHEDULE_H
#define SCHED_PARA_SCHEDULE_H

#include <vector>

#include <sched/common/Api.h>
#include <sched/common/GenericSchedule.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

namespace sched::para {

  struct SCHED_API ParallelTask {
    JobId job;
    MachineId machine;
    Time start;
    Time completion;
  };

  using ParallelSchedule = GenericSchedule<ParallelTask>;

}

#endif // SCHED_PARA_SCHEDULE_H
