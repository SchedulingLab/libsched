#ifndef SCHED_INSERT_NEIHGBORHOOD_H
#define SCHED_INSERT_NEIHGBORHOOD_H

#include <cassert>
#include <algorithm>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>
#include <sched/common/Range.h>

#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  struct SCHED_API InsertNeighborhood {
    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      assert(!input.empty());

      std::size_t max = input.size() - 1;
      Input neighbor;

      do {
        std::size_t orig, dest;

        do {
          orig = random.compute_uniform_integer(std::size_t{0}, max);
          dest = random.compute_uniform_integer(std::size_t{0}, max);
        } while (orig == dest);

        neighbor = input;
        auto val = neighbor[orig];

        if (orig < dest) {
          for (std::size_t i = orig; i < dest; ++i) {
            neighbor[i] = neighbor[i + 1];
          }
        } else {
          for (std::size_t i = orig; i > dest; --i) {
            neighbor[i] = neighbor[i - 1];
          }
        }

        neighbor[dest] = val;
      } while (neighbor == input);


      return neighbor;
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

    template<typename Input>
    bool are_neighbors(const Input& input0, const Input& input1) {
      std::size_t size = input0.size();
      assert(size > 0);
      assert(input1.size() == size);

      std::size_t orig = 0;

      for (; orig < size; ++orig) {
        if (input0[orig] != input1[orig]) {
          break;
        }
      }

      std::size_t dest = size - 1;

      for (; dest > 0; --dest) {
        if (input0[dest] != input1[dest]) {
          break;
        }
      }

      if (orig >= dest) {
        return false;
      }

      bool result = false;

      if (input0[orig] == input1[dest]) {
        result = std::equal(&input0[orig + 1], &input0[dest + 1], &input1[orig], &input1[dest]);
      }

      if (result) {
        return true;
      }

      if (input0[dest] == input1[orig]) {
        result = std::equal(&input0[orig], &input0[dest], &input1[orig + 1], &input1[dest + 1]);
      }

      return result;
    }

  };

  template<>
  struct NeighborhoodTraits<InsertNeighborhood> {
    static std::string name() {
      return "ins";
    }
  };

}

#endif // SCHED_INSERT_NEIHGBORHOOD_H
