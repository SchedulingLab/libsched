// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <print>

#include <gtest/gtest.h>

#include <sched/Time.h>
#include <sched/meta/crossover/LinearOrderCrossover.h>
#include <sched/shop/input/HoistEmptyInput.h>
#include <sched/shop/input/HoistInputConversion.h>
#include <sched/shop/input/HoistInputFmt.h>
#include <sched/shop/input/HoistLoadedInput.h>
#include <sched/shop/instance/HoistSchedulingInstance.h>
#include <sched/shop/instance/HoistSchedulingData.h>

namespace {

  constexpr std::string_view PuInstance = R"json(
{
  "unload": 0,
  "machines": 13,
  "operations": [
    { "processing": { "lo": 120            }, "transport": 31 },
    { "processing": { "lo": 150, "hi": 200 }, "transport": 22 },
    { "processing": { "lo":  90, "hi": 120 }, "transport": 22 },
    { "processing": { "lo": 120, "hi": 180 }, "transport": 22 },
    { "processing": { "lo":  90, "hi": 125 }, "transport": 25 },
    { "processing": { "lo":  30, "hi":  40 }, "transport": 23 },
    { "processing": { "lo":  60, "hi": 120 }, "transport": 22 },
    { "processing": { "lo":  60, "hi": 120 }, "transport": 22 },
    { "processing": { "lo":  45, "hi":  75 }, "transport": 22 },
    { "processing": { "lo": 130            }, "transport": 47 },
    { "processing": { "lo": 120            }, "transport": 27 },
    { "processing": { "lo":  90, "hi": 120 }, "transport": 22 },
    { "processing": { "lo":  30, "hi":  60 }, "transport": 30 }
  ],
  "vehicles": 1,
  "empty": [
    [  0, 11, 14, 16, 14, 19, 22, 24, 26, 29,  6,  8, 10 ],
    [ 11,  0,  2,  5,  2,  8, 10, 13, 15, 17, 10,  3,  1 ],
    [ 14,  2,  0,  2,  0,  5,  8, 10, 13, 15, 12,  6,  3 ],
    [ 16,  5,  2,  0,  2,  3,  5,  8, 10, 13, 15,  8,  6 ],
    [ 14,  2,  0,  2,  0,  5,  8, 10, 13, 15, 12,  6,  3 ],
    [ 19,  8,  5,  3,  5,  0,  3,  5,  7, 10, 18, 11,  9 ],
    [ 22, 10,  8,  5,  8,  3,  0,  2,  5,  7, 20, 14, 11 ],
    [ 24, 13, 10,  8, 10,  5,  2,  0,  2,  5, 23, 16, 14 ],
    [ 26, 15, 13, 10, 13,  7,  5,  2,  0,  2, 25, 19, 16 ],
    [ 29, 17, 15, 13, 15, 10,  7,  5,  2,  0, 27, 21, 19 ],
    [  6, 10, 12, 15, 12, 18, 20, 23, 25, 27,  0,  7,  9 ],
    [  8,  3,  6,  8,  6, 11, 14, 16, 19, 21,  7,  0,  2 ],
    [ 10,  1,  3,  6,  3,  9, 11, 14, 16, 19,  9,  2,  0 ]
  ]
}
)json";

  sched::shop::HoistSchedulingInstance load_instance()
  {
    const std::string instance_json(PuInstance);
    std::istringstream stream(instance_json);

    sched::Json root;
    stream >> root;

    sched::shop::HoistSchedulingData data;
    root.get_to(data);

    return { std::move(data) };
  }

}


TEST(HoistScheduling, InputConversion)
{
  using namespace sched::literals;

  sched::shop::HoistLoadedInput loaded_input = { 0_m, 1_m, 2_m, 3_m, 4_m, 5_m, 6_m, 7_m };
  const std::size_t machine_count = loaded_input.size();

  do {
    sched::shop::HoistEmptyInput empty_input = sched::shop::to_empty_input(loaded_input, machine_count);
    std::optional<sched::shop::HoistLoadedInput> loaded_input_back = sched::shop::to_loaded_input(empty_input, machine_count);
    EXPECT_TRUE(loaded_input_back);
    EXPECT_EQ(loaded_input, *loaded_input_back);
  } while (std::ranges::next_permutation(loaded_input.begin() + 1, loaded_input.end()).found);

}

TEST(HoistScheduling, EmptyInputGeneration)
{
  // sched::shop::HoistSchedulingInstance instance = load_instance();

  constexpr std::size_t MachineCount = 13;

  std::random_device dev;
  sched::Random random(dev());

  auto empty_moves = sched::shop::generate_valid_empty_moves(MachineCount, random);
  sched::shop::HoistEmptyInput input = convert_empty_moves_to_empty_input(empty_moves, MachineCount);

  EXPECT_EQ(input.size(), MachineCount);

  std::println("input: {}", input);
}

TEST(HoistScheduling, EmptyInputCrossover)
{
  std::random_device dev;
  sched::Random random(dev());

  sched::shop::HoistSchedulingInstance instance = load_instance();

  sched::shop::HoistEmptyInput input1 = sched::InputTraits<sched::shop::HoistEmptyInput>::generate_feasible(instance, random);
  sched::shop::HoistEmptyInput input2 = sched::InputTraits<sched::shop::HoistEmptyInput>::generate_feasible(instance, random);

  std::println("input1: {}", input1);
  std::println("input2: {}", input2);

  sched::LinearOrderCrossover<2> crossover = {};

  auto [ child1, child2 ] = crossover(input1, input2, random);

  std::println("child1: {}", child1);
  std::println("child2: {}", child2);

  EXPECT_EQ(child1.length, input1.length);
  EXPECT_EQ(child2.length, input2.length);

  EXPECT_EQ(child1.float_index, input1.float_index);
  EXPECT_EQ(child2.float_index, input2.float_index);

  EXPECT_EQ(child1.size(), instance.machine_count());
  EXPECT_EQ(child2.size(), instance.machine_count());
}
