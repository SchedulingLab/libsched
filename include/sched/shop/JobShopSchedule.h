#ifndef SCHED_SHOP_JOB_SHOP_SCHEDULE_H
#define SCHED_SHOP_JOB_SHOP_SCHEDULE_H

#include <vector>

#include <sched/common/Api.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>
#include <sched/common/GenericSchedule.h>

namespace sched::shop {

  struct SCHED_API JobShopTask {
    OperationId operation;
    MachineId machine;
    Time start;
    Time completion;
  };

  using JobShopSchedule = GenericSchedule<JobShopTask>;

}

#endif // SCHED_SHOP_JOB_SHOP_SCHEDULE_H
