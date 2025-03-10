// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H

#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/helper/JobShopTransportStates.h>

namespace sched::shop {

  template<typename TaskComparator>
  struct JobShopTransportTaskPacketWrapperComparator {

    bool operator()(const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs)
    {
      return cmp(lhs.task, rhs.task);
    }

    static std::string name()
    {
      return TaskComparator::name();
    }

    TaskComparator cmp;
  };

  using JobShopTransportTaskPacketEarliestStartingTime = JobShopTransportTaskPacketWrapperComparator<JobShopTaskEarliestStartingTime>;
  using JobShopTransportTaskPacketLatestStartingTime = JobShopTransportTaskPacketWrapperComparator<JobShopTaskLatestStartingTime>;
  using JobShopTransportTaskPacketEarliestFinishTime = JobShopTransportTaskPacketWrapperComparator<JobShopTaskEarliestFinishTime>;
  using JobShopTransportTaskPacketLatestFinishTime = JobShopTransportTaskPacketWrapperComparator<JobShopTaskLatestFinishTime>;
  using JobShopTransportTaskPacketShortestProcessingTime = JobShopTransportTaskPacketWrapperComparator<JobShopTaskShortestProcessingTime>;
  using JobShopTransportTaskPacketLargestProcessingTime = JobShopTransportTaskPacketWrapperComparator<JobShopTaskLargestProcessingTime>;

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H
