// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H

#include <sched/shop/helper/JobShopTaskComparator.h>
#include <sched/shop/helper/JobShopTransportTaskPacket.h>

namespace sched::shop {

  template<typename Comparator, typename Instance>
  struct JobShopTransportTaskPacketComparatorAdaptor {

    JobShopTransportTaskPacketComparatorAdaptor(const Instance* instance)
    : instance(instance)
    {
    }

    bool operator()(const JobShopTransportTaskPacket& lhs, const JobShopTransportTaskPacket& rhs)
    {
      return comparator(lhs.task, rhs.task, *instance);
    }

    Comparator comparator;
    const Instance* instance = nullptr;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_COMPARATOR_H
