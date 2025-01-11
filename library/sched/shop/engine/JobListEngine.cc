// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/engine/JobListEngine.h>

#include <sched/types/EngineConcepts.h>
#include <sched/shop/instance/FlexibleJobShopInstance.h>
#include <sched/shop/instance/JobShopInstance.h>

static_assert(sched::concepts::EngineFor<sched::shop::JobListEngine, sched::shop::JobShopInstance>);
static_assert(sched::concepts::EngineFor<sched::shop::JobListEngine, sched::shop::FlexibleJobShopInstance>);
