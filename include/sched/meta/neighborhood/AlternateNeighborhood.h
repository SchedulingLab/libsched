// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_ALTERNATE_NEIGHBORHOOD_H
#define SCHED_ALTERNATE_NEIGHBORHOOD_H

#include <cassert>

#include <string>

#include <sched/support/Random.h>
#include <sched/types/InputConcepts.h>
#include <sched/types/NeighborhoodTraits.h>

namespace sched {

  /*
   * Alternate2Neighborhood
   */

  template<typename Neighborhood0, typename Neighborhood1>
  struct Alternate2Neighborhood {

    template<concepts::Input Input, typename Schedule>
    Input operator()(const Input& input, const Schedule& schedule, Random& random)
    {
      std::bernoulli_distribution dist(0.5);
      Input neighbor;

      if (dist(random)) {
        neighbor = neighborhood0(input, schedule, random);
      } else {
        neighbor = neighborhood1(input, schedule, random);
      }

      return neighbor;
    }

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      auto neighbors0 = neighborhood0.generate_many(input, schedule, random, count);
      assert(neighbors0.size() == count);
      auto neighbors1 = neighborhood1.generate_many(input, schedule, random, count);
      assert(neighbors1.size() == count);

      std::bernoulli_distribution dist(0.5);
      std::vector<Input> neighbors;

      for (std::size_t i = 0; i < count; ++i) {
        if (dist(random)) {
          neighbors.push_back(std::move(neighbors0[i]));
        } else {
          neighbors.push_back(std::move(neighbors1[i]));
        }
      }

      return neighbors;
    }

    Neighborhood0 neighborhood0;
    Neighborhood1 neighborhood1;
  };

  template<typename Neighborhood0, typename Neighborhood1>
  struct NeighborhoodTraits<Alternate2Neighborhood<Neighborhood0, Neighborhood1>> {
    static std::string name()
    {
      return NeighborhoodTraits<Neighborhood0>::name() + "_or_" + NeighborhoodTraits<Neighborhood1>::name();
    }
  };

  template<typename Neighborhood0, typename Neighborhood1>
  using Alternate2Mutation = Alternate2Neighborhood<Neighborhood0, Neighborhood1>;

  /*
   * Alternate3Neighborhood
   */

  template<typename Neighborhood0, typename Neighborhood1, typename Neighborhood2>
  struct Alternate3Neighborhood {

    template<concepts::Input Input, typename Schedule>
    Input operator()(const Input& input, const Schedule& schedule, Random& random)
    {
      Input neighbor;
      std::uniform_int_distribution dist(0, 2);

      switch (dist(random)) {
      case 0:
        neighbor = neighborhood0(input, schedule, random);
        break;
      case 1:
        neighbor = neighborhood1(input, schedule, random);
        break;
      case 2:
        neighbor = neighborhood2(input, schedule, random);
        break;
      default:
        assert(false);
        break;
      }

      return neighbor;
    }

    template<concepts::Input Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      auto neighbors0 = neighborhood0.generate_many(input, schedule, random, count);
      assert(neighbors0.size() == count);
      auto neighbors1 = neighborhood1.generate_many(input, schedule, random, count);
      assert(neighbors1.size() == count);
      auto neighbors2 = neighborhood2.generate_many(input, schedule, random, count);
      assert(neighbors2.size() == count);

      std::vector<Input> neighbors;
      std::uniform_int_distribution dist(0, 2);

      for (std::size_t i = 0; i < count; ++i) {
        switch (dist(random)) {
        case 0:
          neighbors.push_back(std::move(neighbors0[i]));
          break;
        case 1:
          neighbors.push_back(std::move(neighbors1[i]));
          break;
        case 2:
          neighbors.push_back(std::move(neighbors2[i]));
          break;
        default:
          assert(false);
          break;
        }
      }

      return neighbors;
    }

    Neighborhood0 neighborhood0;
    Neighborhood1 neighborhood1;
    Neighborhood2 neighborhood2;
  };

  template<typename Neighborhood0, typename Neighborhood1, typename Neighborhood2>
  struct NeighborhoodTraits<Alternate3Neighborhood<Neighborhood0, Neighborhood1, Neighborhood2>> {
    static std::string name()
    {
      return NeighborhoodTraits<Neighborhood0>::name() + "_or_" + NeighborhoodTraits<Neighborhood1>::name() + "_or_" + NeighborhoodTraits<Neighborhood2>::name();
    }
  };

  template<typename Neighborhood0, typename Neighborhood1, typename Neighborhood2>
  using Alternate3Mutation = Alternate3Neighborhood<Neighborhood0, Neighborhood1, Neighborhood2>;

}

#endif // SCHED_ALTERNATE_NEIGHBORHOOD_H
