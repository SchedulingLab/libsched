#ifndef SCHED_SHOP_MUTATION_NEIGHBORHOOD_H
#define SCHED_SHOP_MUTATION_NEIGHBORHOOD_H

#include <string>

#include <sched/common/Random.h>

#include "MutationTraits.h"
#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  template<typename Mutation>
  struct MutationNeighborhood : private Mutation {

    template<typename Input, typename Schedule>
    Input operator()(const Input& input, [[maybe_unused]] const Schedule& schedule, Random& random)
    {
      return Mutation::operator()(input, random);
    }

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }
  };

  template<typename Mutation>
  struct NeighborhoodTraits<MutationNeighborhood<Mutation>> {

    static std::string name()
    {
      return MutationTraits<Mutation>::name();
    }
  };

}

#endif // SCHED_SHOP_MUTATION_NEIGHBORHOOD_H
