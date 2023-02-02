#ifndef SCHED_SHOP_SPLIT_NEIGHBORHOOD_H
#define SCHED_SHOP_SPLIT_NEIGHBORHOOD_H

#include <cassert>
#include <string>
#include <vector>

#include <sched/common/Random.h>

#include "SplitInput.h"
#include "NeighborhoodTraits.h"

namespace sched::shop {

  enum class SplitNeighborhoodKind {
    Either,
    Both,
  };

  template<SplitNeighborhoodKind Kind, typename AssignmentNeighborhood, typename ScheduleNeighborhood>
  struct FlexibleSplitNeighborhood {

    template<typename AssignmentInput, typename ScheduleInput, typename Schedule>
    FlexibleSplitInput<AssignmentInput, ScheduleInput> operator()(const FlexibleSplitInput<AssignmentInput, ScheduleInput>& input, const Schedule& schedule, Random& random) {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> neighbor = input;

      if constexpr (Kind == SplitNeighborhoodKind::Either) {
        std::bernoulli_distribution dist(0.5);

        if (dist(random)) {
          neighbor.assignment = assignment_neighborhood(neighbor.assignment, schedule, random);
        } else {
          neighbor.schedule = schedule_neighborhood(neighbor.schedule, schedule, random);
        }
      } else {
        neighbor.assignment = assignment_neighborhood(neighbor.assignment, schedule, random);
        neighbor.schedule = schedule_neighborhood(neighbor.schedule, schedule, random);
      }

      return neighbor;
    }

    template<typename AssignmentInput, typename ScheduleInput, typename Schedule>
    std::vector<FlexibleSplitInput<AssignmentInput, ScheduleInput>> generate_many(const FlexibleSplitInput<AssignmentInput, ScheduleInput>& input, const Schedule& schedule, Random& random, std::size_t count) {
      auto assignment_neighbors = assignment_neighborhood.generate_many(input.assignment, schedule, random, count);
      assert(assignment_neighbors.size() == count);
      auto schedule_neighbors = schedule_neighborhood.generate_many(input.schedule, schedule, random, count);
      assert(schedule_neighbors.size() == count);

      std::vector<FlexibleSplitInput<AssignmentInput, ScheduleInput>> neighbors;

      for (std::size_t i = 0; i < count; ++i) {
        if constexpr (Kind == SplitNeighborhoodKind::Either) {
          std::bernoulli_distribution dist(0.5);

          if (dist(random)) {
            neighbors.push_back({ std::move(assignment_neighbors[i]), input.schedule });
          } else {
            neighbors.push_back({ input.assignment, std::move(schedule_neighbors[i]) });
          }
        } else {
          neighbors.push_back({ std::move(assignment_neighbors[i]), std::move(schedule_neighbors[i]) });
        }
      }

      return neighbors;
    }

    AssignmentNeighborhood assignment_neighborhood;
    ScheduleNeighborhood schedule_neighborhood;
  };

  template<SplitNeighborhoodKind Kind, typename AssignmentNeighborhood, typename ScheduleNeighborhood>
  struct NeighborhoodTraits<FlexibleSplitNeighborhood<Kind, AssignmentNeighborhood, ScheduleNeighborhood>> {
    static std::string name() {
      return NeighborhoodTraits<AssignmentNeighborhood>::name() + '_'
          + (Kind == SplitNeighborhoodKind::Either ? "or" : "and") + '_'
          + NeighborhoodTraits<ScheduleNeighborhood>::name();
    }
  };

}

#endif // SCHED_SHOP_SPLIT_NEIGHBORHOOD_H
