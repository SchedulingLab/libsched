// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/HoistSchedulingInstance.h>

#include <fstream>

namespace sched::shop {

  HoistSchedulingInstance load_hoist_scheduling_instance(const std::filesystem::path& filename)
  {
    HoistSchedulingData data;

    std::ifstream stream(filename);
    Json root;
    stream >> root;

    root.get_to(data);

    return { std::move(data) };
  }

}
