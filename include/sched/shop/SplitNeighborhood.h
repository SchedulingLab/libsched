#ifndef SCHED_SHOP_SPLIT_NEIGHBORHOOD_H
#define SCHED_SHOP_SPLIT_NEIGHBORHOOD_H

#include <cassert>

#include <string>
#include <vector>

#include <sched/common/Random.h>

#include "NeighborhoodTraits.h"
#include "SplitInput.h"

namespace sched::shop {

  enum class SplitNeighborhoodKind {
    One,
    All,
  };

  template<SplitNeighborhoodKind Kind, typename AssignmentNeighborhood, typename ScheduleNeighborhood>
  struct FlexibleSplitNeighborhood {

    template<typename AssignmentInput, typename ScheduleInput, typename Schedule>
    FlexibleSplitInput<AssignmentInput, ScheduleInput> operator()(const FlexibleSplitInput<AssignmentInput, ScheduleInput>& input, const Schedule& schedule, Random& random)
    {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> neighbor = input;

      if constexpr (Kind == SplitNeighborhoodKind::One) {
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
    std::vector<FlexibleSplitInput<AssignmentInput, ScheduleInput>> generate_many(const FlexibleSplitInput<AssignmentInput, ScheduleInput>& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      auto assignment_neighbors = assignment_neighborhood.generate_many(input.assignment, schedule, random, count);
      assert(assignment_neighbors.size() == count);
      auto schedule_neighbors = schedule_neighborhood.generate_many(input.schedule, schedule, random, count);
      assert(schedule_neighbors.size() == count);

      std::vector<FlexibleSplitInput<AssignmentInput, ScheduleInput>> neighbors;

      for (std::size_t i = 0; i < count; ++i) {
        if constexpr (Kind == SplitNeighborhoodKind::One) {
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
    static std::string name()
    {
      return NeighborhoodTraits<AssignmentNeighborhood>::name() + '_' + NeighborhoodTraits<ScheduleNeighborhood>::name() + '-' + (Kind == SplitNeighborhoodKind::One ? "one" : "all");
    }
  };

  template<SplitNeighborhoodKind Kind, typename ScheduleNeighborhood, typename TransportNeighborhood>
  struct TransportSplitNeighborhood {

    template<typename ScheduleInput, typename TransportInput, typename Schedule>
    TransportSplitInput<ScheduleInput, TransportInput> operator()(const TransportSplitInput<ScheduleInput, TransportInput>& input, const Schedule& schedule, Random& random)
    {
      auto neighbor = input;

      if constexpr (Kind == SplitNeighborhoodKind::One) {
        std::bernoulli_distribution dist(0.5);

        if (dist(random)) {
          neighbor.schedule = schedule_neighborhood(neighbor.schedule, schedule, random);
        } else {
          neighbor.transport = transport_neighborhood(neighbor.transport, schedule, random);
        }
      } else {
        neighbor.schedule = schedule_neighborhood(neighbor.schedule, schedule, random);
        neighbor.transport = transport_neighborhood(neighbor.assignment, schedule, random);
      }

      return neighbor;
    }

    template<typename ScheduleInput, typename TransportInput, typename Schedule>
    std::vector<TransportSplitInput<ScheduleInput, TransportInput>> generate_many(const TransportSplitInput<ScheduleInput, TransportInput>& input, const Schedule& schedule, Random& random, std::size_t count)
    {
      auto schedule_neighbors = schedule_neighborhood.generate_many(input.schedule, schedule, random, count);
      assert(schedule_neighbors.size() == count);
      auto transport_neighbors = transport_neighborhood.generate_many(input.transport, schedule, random, count);
      assert(transport_neighbors.size() == count);

      std::vector<TransportSplitInput<ScheduleInput, TransportInput>> neighbors;

      for (std::size_t i = 0; i < count; ++i) {
        if constexpr (Kind == SplitNeighborhoodKind::One) {
          std::bernoulli_distribution dist(0.5);

          if (dist(random)) {
            neighbors.push_back({ std::move(schedule_neighbors[i]), input.transport });
          } else {
            neighbors.push_back({ input.schedule, std::move(transport_neighbors[i]) });
          }
        } else {
          neighbors.push_back({ std::move(schedule_neighbors[i]), std::move(transport_neighbors[i]) });
        }
      }

      return neighbors;
    }

    ScheduleNeighborhood schedule_neighborhood;
    TransportNeighborhood transport_neighborhood;
  };

  template<SplitNeighborhoodKind Kind, typename ScheduleNeighborhood, typename TransportNeighborhood>
  struct NeighborhoodTraits<TransportSplitNeighborhood<Kind, ScheduleNeighborhood, TransportNeighborhood>> {
    static std::string name()
    {
      return NeighborhoodTraits<ScheduleNeighborhood>::name() + '_' + NeighborhoodTraits<TransportNeighborhood>::name() + '-' + (Kind == SplitNeighborhoodKind::One ? "one" : "all");
    }
  };

}

#endif // SCHED_SHOP_SPLIT_NEIGHBORHOOD_H
