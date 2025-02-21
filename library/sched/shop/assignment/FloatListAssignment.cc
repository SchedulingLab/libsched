// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/assignment/FloatListAssignment.h>

#include <sched/shop/instance/FlexibleJobShopInstance.h>
#include <sched/types/AssignmentConcepts.h>

static_assert(sched::concepts::MachineAssignmentFor<sched::shop::FloatListAssignment, sched::shop::FlexibleJobShopInstance>);
