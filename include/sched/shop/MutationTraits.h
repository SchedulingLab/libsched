#ifndef SCHED_SHOP_MUTATION_TRAITS_H
#define SCHED_SHOP_MUTATION_TRAITS_H

#include "NeighborhoodTraits.h"

namespace sched::shop {

  template<typename Mutation>
  using MutationTraits = NeighborhoodTraits<Mutation>;

}

#endif // SCHED_SHOP_MUTATION_TRAITS_H
