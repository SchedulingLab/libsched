// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_TRANSPORT_SCHEDULE_H
#define SCHED_SHOP_JOB_SHOP_TRANSPORT_SCHEDULE_H

#include <ranges>
#include <vector>

#include <sched/Api.h>
#include <sched/BasicSchedule.h>
#include <sched/Ids.h>
#include <sched/Time.h>

#include <sched/shop/schedule/JobShopSchedule.h>
#include <sched/shop/Transportation.h>

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

    void append_transportation_task(const TransportationTask& transportation_task)
    {
      m_transportation_tasks.push_back(transportation_task);
    }

    using ConstTransportationTaskRange = std::ranges::ref_view<const std::vector<TransportationTask>>;

    ConstTransportationTaskRange transportation_tasks() const
    {
      return std::views::all(m_transportation_tasks);
    }

  private:
    std::vector<TransportationTask> m_transportation_tasks;
  };

}

#endif // SCHED_SHOP_JOB_SHOP_TRANSPORT_SCHEDULE_H
