#ifndef SCHED_SHOP_JOB_SHOP_CRITICAL_PATH_H
#define SCHED_SHOP_JOB_SHOP_CRITICAL_PATH_H

#include <sched/common/Api.h>

#include "JobShopSchedule.h"

namespace sched::shop {

  SCHED_API std::vector<JobShopTask> critical_path(const JobShopSchedule& schedule);

}

#endif // SCHED_SHOP_JOB_SHOP_CRITICAL_PATH_H
