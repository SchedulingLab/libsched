#ifndef SCHED_SHOP_SOLUTION_H
#define SCHED_SHOP_SOLUTION_H

#include <utility>

namespace sched::shop {

  template<typename Engine, typename Criterion, typename Instance>
  struct Solution {
    using Input = typename Engine::Input;
    using Schedule = decltype(std::declval<Engine>()(std::declval<Instance>(), std::declval<Input>()));
    using Fitness = decltype(std::declval<Criterion>()(std::declval<Instance>(), *std::declval<Schedule>()));

    Input input;
    Schedule schedule;
    Fitness fitness;
  };

}

#endif // SCHED_SHOP_SOLUTION_H
