// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_OPTIMAL_ALGORITHM_H
#define SCHED_SHOP_JOB_SHOP_OPTIMAL_ALGORITHM_H

#include <lqp/Solver.h>

#include <sched/Api.h>
#include <sched/shop/instance/JobShopInstance.h>
#include <sched/shop/schedule/JobShopSchedule.h>

namespace sched::shop {

  struct SCHED_API JobShopOptimalAlgorithm {

    JobShopSchedule operator()(const JobShopInstance& instance, lqp::Solver& solver);

  };

}

#endif // SCHED_SHOP_JOB_SHOP_OPTIMAL_ALGORITHM_H
