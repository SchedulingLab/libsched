// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_THREE_NEIGHBORHOOD_H
#define SCHED_THREE_NEIGHBORHOOD_H

#include <cassert>
#include <cstdint>

#include <string>
#include <vector>

#include <sched/meta/input/ThreeInput.h>
#include <sched/meta/neighborhood/NeighborhoodHelper.h>
#include <sched/support/Random.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>
#include <sched/types/NeighborhoodConcepts.h>

namespace sched {

  enum class ThreeNeighborhoodKind : uint8_t {
    One,
    All,
  };

  template<ThreeNeighborhoodKind Kind, typename Neighborhood0, typename Neighborhood1, typename Neighborhood2>
  struct ThreeNeighborhood {

    template<concepts::Input Input0, concepts::Input Input1, concepts::Input Input2, typename Schedule>
    ThreeInput<Input0, Input1, Input2> operator()(const ThreeInput<Input0, Input1, Input2>& input, const Schedule& schedule, Random& random)
    {
      static_assert(concepts::NeighborhoodFor<Neighborhood0, Input0, Schedule>);
      static_assert(concepts::NeighborhoodFor<Neighborhood1, Input1, Schedule>);
      static_assert(concepts::NeighborhoodFor<Neighborhood2, Input2, Schedule>);

      ThreeInput<Input0, Input1, Input2> neighbor = input;

      if constexpr (Kind == ThreeNeighborhoodKind::One) {
        std::uniform_int_distribution dist(0, 2);

        switch (dist(random)) {
          case 0:
            neighbor.input0 = neighborhood0(neighbor.input0, schedule, random);
            break;
          case 1:
            neighbor.input1 = neighborhood1(neighbor.input1, schedule, random);
            break;
          case 2:
            neighbor.input2 = neighborhood2(neighbor.input2, schedule, random);
            break;
          default:
            assert(false);
            break;
        }
      } else {
        neighbor.input0 = neighborhood0(neighbor.input0, schedule, random);
        neighbor.input1 = neighborhood1(neighbor.input1, schedule, random);
        neighbor.input2 = neighborhood2(neighbor.input2, schedule, random);
      }

      return neighbor;
    }

    template<concepts::Input Input0, concepts::Input Input1, concepts::Input Input2, typename Schedule>
    std::vector<ThreeInput<Input0, Input1, Input2>> generate_many(const ThreeInput<Input0, Input1, Input2>& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      static_assert(concepts::NeighborhoodFor<Neighborhood0, Input0, Schedule>);
      static_assert(concepts::NeighborhoodFor<Neighborhood1, Input1, Schedule>);
      static_assert(concepts::NeighborhoodFor<Neighborhood2, Input2, Schedule>);

      auto neighbors0 = neighborhood0.generate_many(input.input0, schedule, random, count);
      assert(neighbors0.size() == count);
      auto neighbors1 = neighborhood1.generate_many(input.input1, schedule, random, count);
      assert(neighbors1.size() == count);
      auto neighbors2 = neighborhood2.generate_many(input.input2, schedule, random, count);
      assert(neighbors2.size() == count);

      std::vector<ThreeInput<Input0, Input1, Input2>> neighbors;

      for (std::size_t i = 0; i < count; ++i) {
        if constexpr (Kind == ThreeNeighborhoodKind::One) {
          std::uniform_int_distribution dist(0, 2);

          switch (dist(random)) {
            case 0:
              neighbors.push_back({ std::move(neighbors0[i]), input.input1, input.input2 });
              break;
            case 1:
              neighbors.push_back({ input.input0, std::move(neighbors1[i]), input.input2 });
              break;
            case 2:
              neighbors.push_back({ input.input0, input.input1, std::move(neighbors2[i]) });
              break;
            default:
              assert(false);
              break;
          }
        } else {
          neighbors.push_back({ std::move(neighbors0[i]), std::move(neighbors1[i]), std::move(neighbors2[i]) });
        }
      }

      return neighbors;
    }

    Neighborhood0 neighborhood0;
    Neighborhood1 neighborhood1;
    Neighborhood2 neighborhood2;
  };

  template<ThreeNeighborhoodKind Kind, typename Neighborhood0, typename Neighborhood1, typename Neighborhood2>
  struct NeighborhoodTraits<ThreeNeighborhood<Kind, Neighborhood0, Neighborhood1, Neighborhood2>> {
    static std::string name()
    {
      return NeighborhoodTraits<Neighborhood0>::name() + '_' + NeighborhoodTraits<Neighborhood1>::name() + '_' + NeighborhoodTraits<Neighborhood2>::name() + '_' + (Kind == ThreeNeighborhoodKind::One ? "one" : "all");
    }
  };

}

#endif // SCHED_THREE_NEIGHBORHOOD_H
