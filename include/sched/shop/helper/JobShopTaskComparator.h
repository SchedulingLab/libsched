// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H
#define SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H

#include <string>

#include <sched/shop/schedule/JobShopSchedule.h>

namespace sched::shop {

  struct JobShopTaskEarliestStartingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const
    {
      return lhs.start < rhs.start;
    }

    static std::string name()
    {
      return "est";
    }
  };

  struct JobShopTaskLatestStartingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const
    {
      return lhs.start > rhs.start;
    }

    static std::string name()
    {
      return "lst";
    }
  };

  struct JobShopTaskEarliestFinishTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const
    {
      return lhs.completion < rhs.completion;
    }

    static std::string name()
    {
      return "eft";
    }
  };

  struct JobShopTaskLatestFinishTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const
    {
      return lhs.completion > rhs.completion;
    }

    static std::string name()
    {
      return "lft";
    }
  };

  struct JobShopTaskShortestProcessingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const
    {
      return (lhs.completion - lhs.start) < (rhs.completion - rhs.start);
    }

    static std::string name()
    {
      return "spt";
    }
  };

  struct JobShopTaskLargestProcessingTime {

    bool operator()(const JobShopTask& lhs, const JobShopTask& rhs) const
    {
      return (lhs.completion - lhs.start) > (rhs.completion - rhs.start);
    }

    static std::string name()
    {
      return "lpt";
    }
  };

}

#endif // SCHED_SHOP_JOB_SHOP_TASK_COMPARATOR_H
