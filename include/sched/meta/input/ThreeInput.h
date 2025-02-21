// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_THREE_INPUT_H
#define SCHED_THREE_INPUT_H

#include <string>

#include <sched/support/Random.h>
#include <sched/types/InputTraits.h>
#include <sched/types/InputConcepts.h>

namespace sched {

  template<concepts::Input Input0, concepts::Input Input1, concepts::Input Input2>
  struct ThreeInput {
    Input0 input0;
    Input1 input1;
    Input2 input2;
  };

  template<concepts::Input Input0, concepts::Input Input1, concepts::Input Input2>
  inline bool operator==(const ThreeInput<Input0, Input1, Input2>& lhs, const ThreeInput<Input0, Input1, Input2>& rhs)
  {
    return lhs.input0 == rhs.input0 && lhs.input1 == rhs.input1 && lhs.input2 == rhs.input2;
  }

  template<concepts::Input Input0, concepts::Input Input1, concepts::Input Input2>
  struct InputTraits<ThreeInput<Input0, Input1, Input2>> {
    static std::string name()
    {
      return InputTraits<Input0>::name() + '_' + InputTraits<Input1>::name() + '_' + InputTraits<Input2>::name();
    }

    template<typename Instance>
    static ThreeInput<Input0, Input1, Input2> generate_input(const Instance& instance)
    {
      static_assert(concepts::InputFor<Input0, Instance>);
      static_assert(concepts::InputFor<Input1, Instance>);
      static_assert(concepts::InputFor<Input2, Instance>);

      ThreeInput<Input0, Input1, Input2> input;
      input.input0 = InputTraits<Input0>::generate_input(instance);
      input.input1 = InputTraits<Input1>::generate_input(instance);
      input.input2 = InputTraits<Input2>::generate_input(instance);
      return input;
    }

    template<typename Instance>
    static ThreeInput<Input0, Input1, Input2> generate_random(const Instance& instance, Random& random)
    {
      static_assert(concepts::InputFor<Input0, Instance>);
      static_assert(concepts::InputFor<Input1, Instance>);
      static_assert(concepts::InputFor<Input2, Instance>);

      ThreeInput<Input0, Input1, Input2> input;
      input.input0 = InputTraits<Input0>::generate_random(instance, random);
      input.input1 = InputTraits<Input1>::generate_random(instance, random);
      input.input2 = InputTraits<Input2>::generate_random(instance, random);
      return input;
    }

    template<typename Instance>
    static ThreeInput<Input0, Input1, Input2> generate_feasible(const Instance& instance, Random& random)
    {
      static_assert(concepts::InputFor<Input0, Instance>);
      static_assert(concepts::InputFor<Input1, Instance>);
      static_assert(concepts::InputFor<Input2, Instance>);

      ThreeInput<Input0, Input1, Input2> input;
      input.input0 = InputTraits<Input0>::generate_feasible(instance, random);
      input.input1 = InputTraits<Input1>::generate_feasible(instance, random);
      input.input2 = InputTraits<Input2>::generate_feasible(instance, random);
      return input;
    }
  };

}

#endif // SCHED_THREE_INPUT_H
