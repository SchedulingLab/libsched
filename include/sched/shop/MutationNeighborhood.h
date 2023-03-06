#ifndef SCHED_SHOP_MUTATION_NEIGHBORHOOD_H
#define SCHED_SHOP_MUTATION_NEIGHBORHOOD_H

#include <string>

#include <sched/common/Random.h>

#include "NeighborhoodTraits.h"
#include "MutationTraits.h"

namespace sched::shop {

  template<typename Mutation>
  struct MutationNeighborhood : Mutation {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random) {
      return Mutation::operator()(input, random);
    }

  };

  template<typename Mutation>
  struct NeighborhoodTraits<MutationNeighborhood<Mutation>> {

    static std::string name() {
      return Mutation::name();
    }

  };

}

#endif // SCHED_SHOP_MUTATION_NEIGHBORHOOD_H
