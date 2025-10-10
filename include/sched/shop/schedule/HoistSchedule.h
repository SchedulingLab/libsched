// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_HOIST_SHEDULE_H
#define SCHED_SHOP_HOIST_SHEDULE_H

#include <sched/Api.h>
#include <sched/BasicSchedule.h>
#include <sched/Ids.h>
#include <sched/Time.h>

namespace sched::shop {

  struct SCHED_API HoistSchedulingTask {
    sched::OperationId operation;
    sched::Time start;
    sched::Time completion;
  };

  struct SCHED_API HoistSchedule : sched::BasicSchedule<HoistSchedulingTask> {
    sched::Time period = sched::TimeMax;
  };

}

#endif // SCHED_SHOP_HOIST_SHEDULE_H
