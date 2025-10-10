// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/support/Array2D.h>

#include <nlohmann/json.hpp>

namespace sched {

  void from_json(const Json& json, Array2D<Time>& data)
  {
    assert(json.is_array());
    assert(json.size() == data.rows());

    for (std::size_t i = 0; i < data.rows(); ++i) {
      assert(json[i].is_array());
      assert(json[i].size() == data.cols());

      for (std::size_t j = 0; j < data.cols(); ++j) {
        json[i][j].get_to(data(i, j));
      }
    }
  }

  void to_json(Json& json, const Array2D<Time>& data)
  {
    json = Json::array();

    for (std::size_t i = 0; i < data.rows(); ++i) {
      Json row_json = Json::array();

      for (std::size_t j = 0; j < data.cols(); ++j) {
        row_json.push_back(data[i][j]);
      }

      json.push_back(std::move(row_json));
    }
  }

}
