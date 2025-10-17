// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_SHOP_PARTITION_H
#define SCHED_SHOP_PARTITION_H

#include <cassert>

#include <vector>

#include <sched/Api.h>

namespace sched::shop {

  class SCHED_API Partition {
  public:
    Partition(std::size_t size, std::vector<std::size_t> partition);

    std::size_t next_index(std::size_t i) const;

    bool empty() const;
    std::size_t size() const;

    const std::vector<std::size_t>& indices() const;

  private:
    std::size_t m_size;
    std::vector<std::size_t> m_partition;
  };

  // all the partitions for the same size
  class SCHED_API PartitionGroup {
  public:
    PartitionGroup() = default;

    PartitionGroup(std::size_t size);

    std::size_t size() const;
    const Partition& operator[](std::size_t i) const;

    auto begin() const
    {
      return m_partitions.begin();
    }

    auto end() const
    {
      return m_partitions.end();
    }

  private:
    static constexpr std::size_t AtLeast = 2;

    void compute_partitions();
    void compute_partition_recursive(std::size_t j, std::vector<std::size_t>& current_partition);

    std::size_t m_size = 0;
    std::vector<Partition> m_partitions;
  };


  // all the partition groups up to a certain size
  class SCHED_API PartitionCollection {
  public:
    PartitionCollection(std::size_t max_size);

    const PartitionGroup& group(std::size_t size) const;
    const Partition& partition(std::size_t size, double float_index) const;

  private:
    std::size_t m_max_size;
    std::vector<PartitionGroup> m_groups;
  };

}

#endif // SCHED_SHOP_PARTITION_H

