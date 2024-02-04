#ifndef SCHED_SHOP_SPLIT_INPUT_H
#define SCHED_SHOP_SPLIT_INPUT_H

#include <string>

#include <sched/common/Random.h>

#include "InputTraits.h"

namespace sched::shop {

  /*
   * FlexibleSplitInput
   */

  template<typename AssignmentInput, typename ScheduleInput>
  struct FlexibleSplitInput {
    AssignmentInput assignment;
    ScheduleInput schedule;
  };

  template<typename AssignmentInput, typename ScheduleInput>
  inline bool operator==(const FlexibleSplitInput<AssignmentInput, ScheduleInput>& lhs, const FlexibleSplitInput<AssignmentInput, ScheduleInput>& rhs)
  {
    return lhs.assignment == rhs.assignment && lhs.schedule == rhs.schedule;
  }

  template<typename AssignmentInput, typename ScheduleInput>
  struct InputTraits<FlexibleSplitInput<AssignmentInput, ScheduleInput>> {
    static std::string name()
    {
      return InputTraits<AssignmentInput>::name() + '_' + InputTraits<ScheduleInput>::name();
    }

    template<typename Instance>
    static FlexibleSplitInput<AssignmentInput, ScheduleInput> generate_input(const Instance& instance)
    {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> input;
      input.assignment = InputTraits<AssignmentInput>::generate_input(instance);
      input.schedule = InputTraits<ScheduleInput>::generate_input(instance);
      return input;
    }

    template<typename Instance>
    static FlexibleSplitInput<AssignmentInput, ScheduleInput> generate_random(const Instance& instance, Random& random)
    {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> input;
      input.assignment = InputTraits<AssignmentInput>::generate_random(instance, random);
      input.schedule = InputTraits<ScheduleInput>::generate_random(instance, random);
      return input;
    }

    template<typename Instance>
    static FlexibleSplitInput<AssignmentInput, ScheduleInput> generate_feasible(const Instance& instance, Random& random)
    {
      FlexibleSplitInput<AssignmentInput, ScheduleInput> input;
      input.assignment = InputTraits<AssignmentInput>::generate_feasible(instance, random);
      input.schedule = InputTraits<ScheduleInput>::generate_feasible(instance, random);
      return input;
    }
  };

  /*
   * TransportSplitInput
   */

  template<typename ScheduleInput, typename TransportInput>
  struct TransportSplitInput {
    ScheduleInput schedule;
    TransportInput transport;
  };

  template<typename ScheduleInput, typename TransportInput>
  inline bool operator==(const TransportSplitInput<ScheduleInput, TransportInput>& lhs, const TransportSplitInput<ScheduleInput, TransportInput>& rhs)
  {
    return lhs.schedule == rhs.schedule && lhs.transport == rhs.transport;
  }

  template<typename ScheduleInput, typename TransportInput>
  struct InputTraits<TransportSplitInput<ScheduleInput, TransportInput>> {
    static std::string name()
    {
      return InputTraits<ScheduleInput>::name() + '_' + InputTraits<TransportInput>::name();
    }

    template<typename Instance>
    static TransportSplitInput<ScheduleInput, TransportInput> generate_input(const Instance& instance)
    {
      TransportSplitInput<ScheduleInput, TransportInput> input;
      input.schedule = InputTraits<ScheduleInput>::generate_input(instance);
      input.transport = InputTraits<TransportInput>::generate_input(instance);
      return input;
    }

    template<typename Instance>
    static TransportSplitInput<ScheduleInput, TransportInput> generate_random(const Instance& instance, Random& random)
    {
      TransportSplitInput<ScheduleInput, TransportInput> input;
      input.schedule = InputTraits<ScheduleInput>::generate_random(instance, random);
      input.transport = InputTraits<TransportInput>::generate_random(instance, random);
      return input;
    }

    template<typename Instance>
    static TransportSplitInput<ScheduleInput, TransportInput> generate_feasible(const Instance& instance, Random& random)
    {
      TransportSplitInput<ScheduleInput, TransportInput> input;
      input.schedule = InputTraits<ScheduleInput>::generate_feasible(instance, random);
      input.transport = InputTraits<TransportInput>::generate_feasible(instance, random);
      return input;
    }
  };

}

#endif // SCHED_SHOP_SPLIT_INPUT_H
