#ifndef SCHED_SHOP_ALTERNATE_MUTATION_H
#define SCHED_SHOP_ALTERNATE_MUTATION_H

#include <string>

#include <sched/common/Random.h>

#include "MutationTraits.h"

namespace sched::shop {

  template<typename Mutation0, typename Mutation1>
  struct AlternateMutation {

    template<typename Input>
    Input operator()(const Input& input, Random& random) {
      std::bernoulli_distribution dist(0.5);
      return dist(random) ? mutation0(input, random) : mutation1(input, random);
    }

    Mutation0 mutation0;
    Mutation1 mutation1;
  };

  template<typename Mutation0, typename Mutation1>
  struct MutationTraits<AlternateMutation<Mutation0, Mutation1>> {
    static std::string name() {
      return MutationTraits<Mutation0>::name() + "_or_" + MutationTraits<Mutation1>::name();
    }
  };

}

#endif // SCHED_SHOP_ALTERNATE_MUTATION_H
