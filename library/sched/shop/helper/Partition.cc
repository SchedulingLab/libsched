// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/helper/Partition.h>

#include <algorithm>
#include <ranges>

namespace sched::shop {

  namespace {

    constexpr std::size_t index_from_float_index(double float_index, std::size_t size)
    {
      return static_cast<std::size_t>(float_index * static_cast<double>(size));
    }

  }


  /*
   * Partition
   */

  Partition::Partition(std::size_t length, std::vector<std::size_t> partition)
  : m_length(length)
  , m_partition(std::move(partition))
  {
    // assert(m_length > 0);
  }

  std::size_t Partition::next_index(std::size_t i) const
  {
    if (m_partition.empty()) {
      if (i + 1 == m_length) {
        return 0;
      }

      return i + 1;
    }

    // find the limit

    std::size_t limit_index = 0;

    while (limit_index < m_partition.size() && m_partition[limit_index] <= i) {
      ++limit_index;
    }

    const std::size_t limit = limit_index < m_partition.size() ? m_partition[limit_index] : m_length;
    assert(i < limit);

    if (i + 1 == limit) {
      if (limit_index > 0) {
        return m_partition[limit_index - 1];
      }

      return 0;
    }

    return i + 1;
  }

  bool Partition::empty() const
  {
    return m_partition.empty();
  }

  std::size_t Partition::length() const
  {
    return m_length;
  }

  std::span<const std::size_t> Partition::indices() const
  {
    return m_partition;
  }

  /*
   * PartitionGroup
   */

  PartitionGroup::PartitionGroup(std::size_t length)
  : m_length(length)
  {
    compute_partitions();
  }

  std::size_t PartitionGroup::size() const
  {
    return m_partitions.size();
  }

  const Partition& PartitionGroup::operator[](std::size_t i) const
  {
    assert(i < m_partitions.size());
    return m_partitions[i];
  }

  const Partition& PartitionGroup::partition(double float_index) const
  {
    assert(0.0 <= float_index && float_index < 1.0);
    const std::size_t index = index_from_float_index(float_index, m_partitions.size());
    assert(index < m_partitions.size());
    return m_partitions[index];
  }

  std::size_t PartitionGroup::find_partition(const Partition& partition) const
  {
    assert(partition.length() == m_length);

    for (auto [ index, partition_for_index ] : std::views::enumerate(m_partitions)) {
      if (std::ranges::equal(partition.indices(), partition_for_index.indices())) {
        return index;
      }
    }

    return m_partitions.size();
  }

  void PartitionGroup::compute_partitions()
  {
    std::vector<std::size_t> partition;

    if (m_length < AtLeast) {
      m_partitions.emplace_back(m_length, partition);
    } else {
      compute_partition_recursive(0, partition);
    }

    assert(m_partitions.size() == partition_group_count(m_length));
  }

  void PartitionGroup::compute_partition_recursive(std::size_t j, std::vector<std::size_t>& current_partition) // NOLINT(misc-no-recursion)
  {
    m_partitions.emplace_back(m_length, current_partition);

    for (std::size_t i = j + AtLeast; i <= m_length - AtLeast; ++i) {
      current_partition.push_back(i);
      compute_partition_recursive(i, current_partition);
      current_partition.pop_back();
    }
  }

  /*
   * PartitionCollection
   */

  PartitionCollection::PartitionCollection(std::size_t max_length)
  : m_max_length(max_length)
  {
    for (std::size_t i = 0; i <= m_max_length; ++i) {
      m_groups.emplace_back(i);
    }
  }

  const PartitionGroup& PartitionCollection::group(std::size_t length) const
  {
    assert(length <= m_max_length);
    return m_groups[length];
  }

  const Partition& PartitionCollection::partition(std::size_t length, double float_index) const
  {
    return group(length).partition(float_index);
  }

  /*
   * Useful functions
   */

  std::size_t partition_group_count(std::size_t length)
  {
    if (length <= 3) {
      return 1;
    }

    std::size_t u = 1;
    std::size_t v = 1;

    for (std::size_t i = 3; i < length; ++i) {
      const std::size_t w = u + v;
      u = v;
      v = w;
    }

    return v;
  }

  bool reference_same_partition(double float_index0, double float_index1, std::size_t length)
  {
    const std::size_t count = partition_group_count(length);
    const std::size_t index0 = index_from_float_index(float_index0, count);
    const std::size_t index1 = index_from_float_index(float_index1, count);
    return index0 == index1;
  }

}
