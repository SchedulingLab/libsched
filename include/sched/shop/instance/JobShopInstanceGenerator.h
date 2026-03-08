// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_JOB_SHOP_GENERATOR_INSTANCE_H
#define SCHED_SHOP_JOB_SHOP_GENERATOR_INSTANCE_H

#include <cstdint>

#include <span>

#include <sched/Api.h>
#include <sched/shop/instance/JobShopInstance.h>

namespace sched::shop {

  struct SCHED_API TaillardInstance {
    std::size_t jobs;
    std::size_t machines;
    int32_t time_seed;
    int32_t machine_seed;
  };

  SCHED_API JobShopInstance compute_taillard_legacy_instance(const TaillardInstance& instance);

  constexpr std::size_t TaillardInstanceCount = 80;
  SCHED_API std::span<const TaillardInstance, TaillardInstanceCount> load_taillard_benchmark();

  struct SCHED_API DemirkolMehtaUzsoyInstance {
    std::size_t jobs;
    std::size_t machines;
    int32_t seed;
  };

  SCHED_API JobShopInstance compute_dmu_legacy_instance(const DemirkolMehtaUzsoyInstance& instance);


}

#endif // SCHED_SHOP_JOB_SHOP_GENERATOR_INSTANCE_H
