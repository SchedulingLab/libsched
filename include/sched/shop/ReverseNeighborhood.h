#ifndef SCHED_REVERSE_NEIHGBORHOOD_H
#define SCHED_REVERSE_NEIHGBORHOOD_H

#include "MutationNeighborhood.h"
#include "ReverseMutation.h"

namespace sched::shop {

  using ReverseNeighborhood = MutationNeighborhood<ReverseMutation>;
  using CappedReverseNeighborhood = MutationNeighborhood<CappedReverseMutation>;

}

#endif // SCHED_REVERSE_NEIHGBORHOOD_H
