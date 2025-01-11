// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_EXTENT_H
#define SCHED_EXTENT_H

namespace sched {

  template<typename T>
  struct Extent {
    T lo; // inclusive
    T hi; // inclusive

    bool contains(T value) const noexcept
    {
      return lo <= value && value <= hi;
    }
  };

}

#endif // SCHED_EXTENT_H
