// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_SCHEDULE_H
#define SCHED_PARA_SCHEDULE_H

#include <sched/Api.h>
#include <sched/BasicSchedule.h>
#include <sched/Ids.h>
#include <sched/Time.h>

namespace sched::para {

  struct SCHED_API ParallelTask {
    JobId job;
    MachineId machine;
    Time start;
    Time completion;
  };

  using ParallelSchedule = BasicSchedule<ParallelTask>;

}

#endif // SCHED_PARA_SCHEDULE_H
