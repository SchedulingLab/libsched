#ifndef SCHED_SHOP_GENERAL_FLEXIBLE_JOB_SHOP_SCHEDULE_H
#define SCHED_SHOP_GENERAL_FLEXIBLE_JOB_SHOP_SCHEDULE_H

#include <vector>

#include <sched/common/Api.h>
#include <sched/common/GenericSchedule.h>
#include <sched/common/Ids.h>
#include <sched/common/Time.h>

#include "JobShopSchedule.h"
#include "Transportation.h"

namespace sched::shop {

  struct TransportationTask {
    TransportationId transportation_resource;
    TransportationKind transportation_kind;
    MachineId origin;
    MachineId target;
    Time start;
    Time completion;
  };

  class JobShopTransportSchedule : public JobShopSchedule {
  public:

    void append_transportation_task(const TransportationTask& transportation_task) {
      m_transportation_tasks.push_back(transportation_task);
    }

    using ConstTransportationTaskRange = IteratorRange<std::vector<TransportationTask>::const_iterator>;

    ConstTransportationTaskRange transportation_tasks() const {
      return make_iterator_range(m_transportation_tasks);
    }

  private:
    std::vector<TransportationTask> m_transportation_tasks;
  };

}

#endif // SCHED_SHOP_GENERAL_FLEXIBLE_JOB_SHOP_SCHEDULE_H
