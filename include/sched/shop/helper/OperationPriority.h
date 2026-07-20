// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_OPERATION_PRIORITY_H
#define SCHED_SHOP_OPERATION_PRIORITY_H

#include <map>

#include <sched/Ids.h>

namespace sched::shop {

  // TODO: change to flat_map when gcc-15 available on Debian stable
  using OperationPriority = std::map<OperationId, double>;

}

#endif // SCHED_SHOP_OPERATION_PRIORITY_H
