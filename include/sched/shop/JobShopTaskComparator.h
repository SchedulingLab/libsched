#ifndef SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H
#define SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H

#include "JobShopSchedule.h"

namespace sched::shop {

  struct JobShopTaskEarliestStartingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const {
      return lhs.start < rhs.start;
    }

  };

  struct JobShopTaskLatestStartingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const {
      return lhs.start > rhs.start;
    }

  };

  struct JobShopTaskEarliestFinishTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const {
      return lhs.completion < rhs.completion;
    }

  };

  struct JobShopTaskLatestFinishTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const {
      return lhs.completion > rhs.completion;
    }

  };

  struct JobShopTaskShortestProcessingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const {
      return (lhs.completion - lhs.start) < (rhs.completion - rhs.start);
    }

  };

  struct JobShopTaskLargestProcessingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const {
      return (lhs.completion - lhs.start) > (rhs.completion - rhs.start);
    }

  };

}


#endif // SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H
