// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/meta/Instance.h>

#include <iterator>

#include <sched/Ids.h>

static_assert(std::ranges::bidirectional_range<sched::JobRange>);
static_assert(std::bidirectional_iterator<sched::JobRange::Iterator>);

static_assert(std::ranges::bidirectional_range<sched::MachineRange>);
static_assert(std::bidirectional_iterator<sched::MachineRange::Iterator>);

static_assert(std::ranges::bidirectional_range<sched::VehicleRange>);
static_assert(std::bidirectional_iterator<sched::VehicleRange::Iterator>);

static_assert(std::ranges::bidirectional_range<sched::OperationRange>);
static_assert(std::bidirectional_iterator<sched::OperationRange::Iterator>);

