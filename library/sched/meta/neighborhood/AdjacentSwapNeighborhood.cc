// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/meta/neighborhood/AdjacentSwapNeighborhood.h>

#include <sched/types/NeighborhoodConcepts.h>
#include <sched/shop/input/JobListInput.h>

static_assert(sched::concepts::NeighborhoodFor<sched::AdjacentSwapNeighborhood, sched::shop::JobListInput>);
