#ifndef SCHED_REVERSE_NEIHGBORHOOD_H
#define SCHED_REVERSE_NEIHGBORHOOD_H

#include <string>

#include <sched/common/Api.h>
#include <sched/common/Random.h>

#include "MutationNeighborhood.h"
#include "NeighborhoodHelper.h"
#include "NeighborhoodTraits.h"
#include "ReverseMutation.h"

namespace sched::shop {

  struct SCHED_API ReverseNeighborhood : MutationNeighborhood<ReverseMutation> {

    template<typename Input, typename Schedule>
    std::vector<Input> generate_many(const Input& input, const Schedule& schedule, Random& random, std::size_t count) {
      return NeighborhoodHelper::generate_many(*this, input, schedule, random, count);
    }

  };

  template<>
  struct NeighborhoodTraits<ReverseNeighborhood> {
    static std::string name() {
      return MutationTraits<ReverseMutation>::name();
    }
  };

}

#endif // SCHED_REVERSE_NEIHGBORHOOD_H
