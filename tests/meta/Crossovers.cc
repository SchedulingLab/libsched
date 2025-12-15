// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <gtest/gtest.h>

#include <sched/meta/crossover/OrderCrossover.h>
#include <sched/meta/crossover/LinearOrderCrossover.h>

TEST(CrossoverTest, OrderCrossover1) {
  static constexpr std::size_t InputSize = 8;
  using Input = std::array<int, InputSize>;
  const Input parent0 = { 1, 2, 3, 4, 5, 6, 7, 8 };
  const Input parent1 = { 2, 4, 6, 8, 7, 5, 3, 1 };

  static constexpr std::size_t PointCount = 2;

  const std::array<std::size_t, PointCount> points = { 2, 5 };
  sched::OrderCrossover1<PointCount> crossover = {};

  auto [ child0, child1 ] = crossover.compute_crossover(parent0, parent1, points);

  const Input expected0 = { 4, 5, 6, 8, 7, 1, 2, 3 };
  const Input expected1 = { 8, 7, 3, 4, 5, 1, 2, 6 };

  EXPECT_EQ(child0, expected0);
  EXPECT_EQ(child1, expected1);
}

TEST(CrossoverTest, LinearOrderCrossover) {
  static constexpr std::size_t InputSize = 9;
  using Input = std::array<int, InputSize>;
  const Input parent0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const Input parent1 = { 6, 7, 9, 4, 3, 2, 1, 5, 8 };

  static constexpr std::size_t PointCount = 2;

  const std::array<std::size_t, PointCount> points = { 3, 6 };
  sched::LinearOrderCrossover<PointCount> crossover = {};

  auto [ child0, child1 ] = crossover.compute_crossover(parent0, parent1, points);

  const Input expected0 = { 1, 5, 6, 4, 3, 2, 7, 8, 9 };
  const Input expected1 = { 7, 9, 3, 4, 5, 6, 2, 1, 8 };

  EXPECT_EQ(child0, expected0);
  EXPECT_EQ(child1, expected1);
}


