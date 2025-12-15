// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/IdsFmt.h>

static_assert(std::formattable<sched::JobId, char>);
static_assert(std::formattable<sched::OperationId, char>);
static_assert(std::formattable<sched::MachineId, char>);
static_assert(std::formattable<sched::VehicleId, char>);
