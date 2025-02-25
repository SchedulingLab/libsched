// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_JSON_H
#define SCHED_JSON_H

#include <iosfwd>
#include <nlohmann/json_fwd.hpp>

namespace sched {

  void dump_json(std::ostream& os, const nlohmann::json& j);

}

#endif // SCHED_JSON_H
