// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2022-2025 Julien Bernard
#ifndef SCHED_CSV_H
#define SCHED_CSV_H

#include <iostream>

namespace sched {

  inline std::ostream& sep(std::ostream& os)
  {
    return os << ',';
  }

  inline std::ostream& nl(std::ostream& os)
  {
    return os << '\n';
  }

}

#endif // SCHED_CSV_H
