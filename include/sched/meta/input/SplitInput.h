#ifndef SCHED_SPLIT_INPUT_H
#define SCHED_SPLIT_INPUT_H

#include <string>

#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>
#include <sched/types/InputConcepts.h>

namespace sched {

  template<concepts::Input Input0, concepts::Input Input1>
  struct SplitInput {
    Input0 input0;
    Input1 input1;
  };

  template<concepts::Input Input0, concepts::Input Input1>
  inline bool operator==(const SplitInput<Input0, Input1>& lhs, const SplitInput<Input0, Input1>& rhs)
  {
    return lhs.input0 == rhs.input0 && lhs.input1 == rhs.input1;
  }

  template<concepts::Input Input0, concepts::Input Input1>
  struct InputTraits<SplitInput<Input0, Input1>> {
    static std::string name()
    {
      return InputTraits<Input0>::name() + '_' + InputTraits<Input1>::name();
    }

    template<typename Instance>
    static SplitInput<Input0, Input1> generate_input(const Instance& instance)
    {
      static_assert(concepts::InputFor<Input0, Instance>);
      static_assert(concepts::InputFor<Input1, Instance>);

      SplitInput<Input0, Input1> input;
      input.input0 = InputTraits<Input0>::generate_input(instance);
      input.input1 = InputTraits<Input1>::generate_input(instance);
      return input;
    }

    template<typename Instance>
    static SplitInput<Input0, Input1> generate_random(const Instance& instance, Random& random)
    {
      static_assert(concepts::InputFor<Input0, Instance>);
      static_assert(concepts::InputFor<Input1, Instance>);

      SplitInput<Input0, Input1> input;
      input.input0 = InputTraits<Input0>::generate_random(instance, random);
      input.input1 = InputTraits<Input1>::generate_random(instance, random);
      return input;
    }

    template<typename Instance>
    static SplitInput<Input0, Input1> generate_feasible(const Instance& instance, Random& random)
    {
      static_assert(concepts::InputFor<Input0, Instance>);
      static_assert(concepts::InputFor<Input1, Instance>);

      SplitInput<Input0, Input1> input;
      input.input0 = InputTraits<Input0>::generate_feasible(instance, random);
      input.input1 = InputTraits<Input1>::generate_feasible(instance, random);
      return input;
    }
  };

}

#endif // SCHED_SPLIT_INPUT_H
