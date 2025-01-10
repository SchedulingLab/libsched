#ifndef SCHED_SHOP_JOB_SHOP_SCHEDULE_H
#define SCHED_SHOP_JOB_SHOP_SCHEDULE_H

#include <sched/Api.h>
#include <sched/BasicSchedule.h>
#include <sched/Ids.h>
#include <sched/Time.h>

namespace sched::shop {

  struct SCHED_API JobShopTask {
    OperationId operation;
    MachineId machine;
    Time start;
    Time completion;
  };

  using JobShopSchedule = BasicSchedule<JobShopTask>;

  SCHED_API bool is_schedule_valid(const JobShopSchedule& schedule);

  SCHED_API JobShopSchedule make_schedule_active(const JobShopSchedule& original_schedule);

}

#endif // SCHED_SHOP_JOB_SHOP_SCHEDULE_H
