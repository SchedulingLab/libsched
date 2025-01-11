// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_CRITICAL_PATH_H
#define SCHED_SHOP_JOB_SHOP_CRITICAL_PATH_H

#include <sched/Api.h>
#include <sched/shop/schedule/JobShopSchedule.h>

namespace sched::shop {

  SCHED_API std::vector<JobShopTask> critical_path(const JobShopSchedule& schedule);

}

#endif // SCHED_SHOP_JOB_SHOP_CRITICAL_PATH_H
