// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/instance/FlexibleJobShopTransportInstance.h>

#include <fstream>

#include <sched/types/ShopInstanceConcepts.h>

static_assert(sched::concepts::ShopTransportInstance<sched::shop::FlexibleJobShopTransportInstance>);

namespace sched::shop {

  FlexibleJobShopTransportInstance import_fjspt_json(const std::filesystem::path& filename, TransportationMode mode)
  {
    FlexibleJobShopTransportData data;

    std::ifstream stream(filename);
    nlohmann::json root;
    stream >> root;

    root.get_to(data);

    return { std::move(data), mode };
  }

}
