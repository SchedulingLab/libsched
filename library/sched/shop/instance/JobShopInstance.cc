// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/JobShopInstance.h>

#include <sched/types/ShopInstanceConcepts.h>

static_assert(sched::concepts::ShopInstance<sched::shop::JobShopInstance>);
