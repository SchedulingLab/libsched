#ifndef SCHED_PARA_PARALLEL_JOB_H
#define SCHED_PARA_PARALLEL_JOB_H

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

namespace sched::para {

  struct SCHED_API ParallelJob {
    JobId id;
    Time processing_time;
  };

}

#endif // SCHED_PARA_PARALLEL_JOB_H
