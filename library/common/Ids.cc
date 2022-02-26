#include <sched/common/Ids.h>

#include <iostream>

namespace sched {

  std::ostream& operator<<(std::ostream& os, MachineId id) {
    return os << to_index(id);
  }

  std::ostream& operator<<(std::ostream& os, JobId id) {
    return os << to_index(id);
  }

  std::ostream& operator<<(std::ostream& os, OperationId id) {
    return os << to_index(id.job) << '[' << id.index << ']';
  }

}
