#ifndef SCHED_TOOLS_CSV_H
#define SCHED_TOOLS_CSV_H

#include <iostream>

namespace sched {

  inline
  std::ostream& sep(std::ostream& os) {
    return os << ',';
  }

  inline
  std::ostream& nl(std::ostream& os) {
    return os << '\n';
  }

}

#endif // SCHED_TOOLS_CSV_H
