// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_H

#include <sched/Api.h>
#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/schedule/JobShopTransportSchedule.h>

namespace sched::shop {

  struct SCHED_API JobShopTransportTaskPacket {
    JobShopTask task;
    TransportationTask empty_task;
    TransportationTask loaded_task;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_TASK_PACKET_H
