#ifndef SCHED_SHOP_ALTERNATE_MUTATION_H
#define SCHED_SHOP_ALTERNATE_MUTATION_H

#include "AlternateNeighborhood.h"

namespace sched::shop {

  template<typename Mutation0, typename Mutation1>
  using AlternateMutation = AlternateNeighborhood<Mutation0, Mutation1>;

}

#endif // SCHED_SHOP_ALTERNATE_MUTATION_H
