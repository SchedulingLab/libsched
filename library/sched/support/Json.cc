// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/support/Json.h>

#include <cassert>

#include <algorithm>
#include <iostream>

#include <nlohmann/json.hpp>

namespace sched {

  namespace {
    constexpr std::size_t IndentWidth = 2;

    struct JsonSerializer {

      void serialize(std::ostream& os, const Json& j)
      {
        using JsonValueType = Json::value_t;
        bool first = true;

        switch (j.type()) {
          case JsonValueType::boolean:
          case JsonValueType::number_integer:
          case JsonValueType::number_unsigned:
          case JsonValueType::number_float:
          case JsonValueType::null:
          case JsonValueType::string:
            inline_serialize(os, j);
            break;

          case JsonValueType::object:
            if (std::ranges::all_of(j.items(), [](const auto& item) { return item.value().is_primitive(); }) || j.size() == 1) {
              inline_serialize(os, j);
            } else {
              os << "{\n";
              ++indent;
              first = true;

              for (const auto& [ key, value ] : j.items()) {
                if (first) {
                  first = false;
                } else {
                  os << ",\n";
                }

                os << std::string(indent * IndentWidth, ' ');
                serialize(os, key);
                os << ": ";
                serialize(os, value);
              }

              --indent;
              os << '\n' << std::string(indent * IndentWidth, ' ') << "}";
            }
            break;

          case JsonValueType::array:
            if (std::ranges::all_of(j, [](const Json& item) { return item.is_primitive(); }) || j.size() == 1) {
              inline_serialize(os, j);
            } else {
              os << "[\n";
              ++indent;
              first = true;

              for (const auto& element : j) {
                if (first) {
                  first = false;
                } else {
                  os << ",\n";
                }

                os << std::string(indent * IndentWidth, ' ');
                serialize(os, element);
              }

              --indent;
              os << '\n' << std::string(indent * IndentWidth, ' ') << ']';
            }
            break;

          case JsonValueType::binary:
          case JsonValueType::discarded:
            assert(false);
            break;

        }
      }

      void inline_serialize(std::ostream& os, const Json& j)
      {
        using JsonValueType = Json::value_t;
        bool first = true;

        switch (j.type()) {
          case JsonValueType::null:
            os << "null";
            break;

          case JsonValueType::boolean:
            os << (j.get<bool>() ? "true" : "false");
            break;

          case JsonValueType::number_integer:
            os << j.get<int64_t>();
            break;

          case JsonValueType::number_unsigned:
            os << j.get<uint64_t>();
            break;

          case JsonValueType::number_float:
            os << j.get<double>();
            break;

          case JsonValueType::string:
            os << std::quoted(j.get<std::string>());
            break;

          case JsonValueType::object:
            os << "{ ";
            first = true;

            for (const auto& [ key, value ] : j.items()) {
              if (first) {
                first = false;
              } else {
                os << ", ";
              }

              inline_serialize(os, key);
              os << ": ";
              inline_serialize(os, value);
            }

            os << " }";
            break;

          case JsonValueType::array:
            os << "[ ";
            first = true;

            for (const auto& element : j) {
              if (first) {
                first = false;
              } else {
                os << ", ";
              }

              inline_serialize(os, element);
            }

            os << " ]";
            break;

          case JsonValueType::binary:
          case JsonValueType::discarded:
            assert(false);
            break;
        }
      }

      std::size_t indent = 0;
    };

  }


  void dump_json(std::ostream& os, const Json& j)
  {
    JsonSerializer serializer;
    serializer.serialize(os, j);
  }

}
