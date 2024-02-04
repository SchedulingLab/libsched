#ifndef SCHED_SHOP_JOB_SHOP_SCHEDULE_H
#define SCHED_SHOP_JOB_SHOP_SCHEDULE_H

#include <sched/common/Api.h>
#include <sched/common/GenericSchedule.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

namespace sched::shop {

  struct SCHED_API JobShopTask {
    OperationId operation;
    MachineId machine;
    Time start;
    Time completion;
  };

  using JobShopSchedule = GenericSchedule<JobShopTask>;

  SCHED_API bool is_schedule_valid(const JobShopSchedule& schedule);

  SCHED_API JobShopSchedule make_schedule_active(const JobShopSchedule& original_schedule);

}

#endif // SCHED_SHOP_JOB_SHOP_SCHEDULE_H
