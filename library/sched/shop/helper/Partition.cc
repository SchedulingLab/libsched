// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard

#include <sched/shop/helper/Partition.h>

namespace sched::shop {

  /*
   * Partition
   */

  Partition::Partition(std::size_t size, std::vector<std::size_t> partition)
  : m_size(size)
  , m_partition(std::move(partition))
  {
    assert(m_size > 0);
  }

  std::size_t Partition::next_index(std::size_t i) const
  {
    if (m_partition.empty()) {
      if (i + 1 == m_size) {
        return 0;
      }

      return i + 1;
    }

    // find the limit

    std::size_t limit_index = 0;

    while (limit_index < m_partition.size() && m_partition[limit_index] <= i) {
      ++limit_index;
    }

    const std::size_t limit = limit_index < m_partition.size() ? m_partition[limit_index] : m_size;
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

  std::size_t Partition::size() const
  {
    return m_size;
  }

  const std::vector<std::size_t>& Partition::indices() const
  {
    return m_partition;
  }

  /*
   * PartitionGroup
   */

  PartitionGroup::PartitionGroup(std::size_t size)
  : m_size(size)
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

  void PartitionGroup::compute_partitions()
  {
    std::vector<std::size_t> partition;
    compute_partition_recursive(0, partition);
  }

  void PartitionGroup::compute_partition_recursive(std::size_t j, std::vector<std::size_t>& current_partition) // NOLINT(misc-no-recursion)
  {
    m_partitions.emplace_back(m_size, current_partition);

    if (m_size < AtLeast) {
      return;
    }

    for (std::size_t i = j + AtLeast; i <= m_size - AtLeast; ++i) {
      current_partition.push_back(i);
      compute_partition_recursive(i, current_partition);
      current_partition.pop_back();
    }
  }

  /*
   * PartitionCollection
   */

  PartitionCollection::PartitionCollection(std::size_t max_size)
  : m_max_size(max_size)
  {
    for (std::size_t i = 0; i <= m_max_size; ++i) {
      m_groups.emplace_back(i);
    }
  }

  const PartitionGroup& PartitionCollection::group(std::size_t size) const
  {
    assert(size <= m_max_size);
    return m_groups[size];
  }

  const Partition& PartitionCollection::partition(std::size_t size, double float_index) const
  {
    assert(size <= m_max_size);
    assert(0.0 <= float_index && float_index < 1.0);

    const PartitionGroup& group = m_groups[size];

    const auto index = static_cast<std::size_t>(float_index * static_cast<double>(group.size()));
    assert(index < group.size());

    return group[index];
  }

}
