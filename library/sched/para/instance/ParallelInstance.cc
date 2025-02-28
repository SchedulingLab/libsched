// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/para/instance/ParallelInstance.h>

#include <sched/types/ShopInstanceConcepts.h>

static_assert(sched::concepts::Instance<sched::para::ParallelInstance>);
