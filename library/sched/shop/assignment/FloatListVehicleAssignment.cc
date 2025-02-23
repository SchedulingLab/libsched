// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/assignment/FloatListVehicleAssignment.h>

#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>
#include <sched/types/AssignmentConcepts.h>

static_assert(sched::concepts::VehicleAssignmentFor<sched::shop::FloatListVehicleAssignment, sched::shop::FlexibleJobShopTransportInstance>);
