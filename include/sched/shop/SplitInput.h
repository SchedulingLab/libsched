#ifndef SCHED_SHOP_SPLIT_INPUT_H
#define SCHED_SHOP_SPLIT_INPUT_H

namespace sched::shop {

  template<typename AssignmentInput, typename ScheduleInput>
  struct FlexibleSplitInput {
    AssignmentInput assignment;
    ScheduleInput schedule;
  };

}

#endif // SCHED_SHOP_SPLIT_INPUT_H
