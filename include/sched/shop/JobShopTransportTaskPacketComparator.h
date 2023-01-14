#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H

#include "JobShopTransportStates.h"
#include "JobShopTaskComparator.h"

namespace sched::shop {

  template<typename TaskComparator>
  struct JobShopTransportTaskPacketWrapperComparator {

    bool operator()(const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs) {
      return cmp(lhs.task, rhs.task);
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
