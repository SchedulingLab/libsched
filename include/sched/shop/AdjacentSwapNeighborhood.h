#ifndef SCHED_SHOP_ADJACENT_SWAP_NEIGHBORHOOD_H
#define SCHED_SHOP_ADJACENT_SWAP_NEIGHBORHOOD_H

#include <cassert>
#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "AdjacentSwapMutation.h"
#include "MutationNeighborhood.h"
#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  struct SCHED_API AdjacentSwapNeighborhood : MutationNeighborhood<AdjacentSwapMutation> {

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<AdjacentSwapNeighborhood> {
    static std::string name() {
      return MutationTraits<AdjacentSwapMutation>::name();
    }
  };

}

#endif // SCHED_SHOP_ADJACENT_SWAP_NEIGHBORHOOD_H
