// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_EXPORT_H
#define SCHED_SHOP_EXPORT_H

#include <iosfwd>

#include <sched/Api.h>

#include <sched/shop/schedule/JobShopSchedule.h>

namespace sched::shop {

  SCHED_API void export_schedule(const JobShopSchedule& schedule, std::size_t machines, std::ostream& out);

}

#endif // SCHED_SHOP_EXPORT_H
