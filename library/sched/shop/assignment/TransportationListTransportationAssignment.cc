// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/assignment/TransportationListTransportationAssignment.h>

#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>
#include <sched/types/AssignmentConcepts.h>

static_assert(sched::concepts::VehicleAssignmentFor<sched::shop::TransportationListTransportationAssignment, sched::shop::FlexibleJobShopTransportInstance>);
