// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_MACHINE_OPERATIONS_H
#define SCHED_SHOP_MACHINE_OPERATIONS_H

#include <vector>

#include <sched/Ids.h>

namespace sched::shop {

  using MachineOperations = std::vector<std::vector<OperationId>>;

}

#endif // SCHED_SHOP_MACHINE_OPERATIONS_H
