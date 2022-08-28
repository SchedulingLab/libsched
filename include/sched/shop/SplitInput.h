#ifndef SCHED_SHOP_SPLIT_INPUT_H
#define SCHED_SHOP_SPLIT_INPUT_H

#include <sched/common/Random.h>

#include "InputTraits.h"

namespace sched::shop {

  template<typename AssignmentInput, typename ScheduleInput>
  struct FlexibleSplitInput {
    AssignmentInput assignment;
    ScheduleInput schedule;
  };

  template<typename AssignmentInput, typename ScheduleInput>
  struct InputTraits<FlexibleSplitInput<AssignmentInput, ScheduleInput>> {

    template<typename Instance>
    static FlexibleSplitInput<AssignmentInput, ScheduleInput> generate_input(const Instance& instance) {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> input;
      input.assignment = InputTraits<AssignmentInput>::generate_input(instance);
      input.schedule = InputTraits<ScheduleInput>::generate_input(instance);
      return input;
    }

    template<typename Instance>
    static FlexibleSplitInput<AssignmentInput, ScheduleInput> generate_random(const Instance& instance, Random& random) {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> input;
      input.assignment = InputTraits<AssignmentInput>::generate_random(instance, random);
      input.schedule = InputTraits<ScheduleInput>::generate_random(instance, random);
      return input;
    }

    template<typename Instance>
    static FlexibleSplitInput<AssignmentInput, ScheduleInput> generate_feasible(const Instance& instance, Random& random) {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> input;
      input.assignment = InputTraits<AssignmentInput>::generate_feasible(instance, random);
      input.schedule = InputTraits<ScheduleInput>::generate_feasible(instance, random);
      return input;
    }

  };

}

#endif // SCHED_SHOP_SPLIT_INPUT_H
