// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_PARA_PARALLEL_JOB_H
#define SCHED_PARA_PARALLEL_JOB_H

#include <sched/Api.h>
#include <sched/Ids.h>
#include <sched/Time.h>

namespace sched::para {

  struct SCHED_API ParallelJob {
    JobId id;
    Time processing_time;
  };

}

#endif // SCHED_PARA_PARALLEL_JOB_H
