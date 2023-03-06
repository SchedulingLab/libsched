#ifndef SCHED_SHOP_ORDER_CROSSOVER_H
#define SCHED_SHOP_ORDER_CROSSOVER_H

#include <tuple>

namespace sched::shop {

  struct OrderCrossover {

    template<typename Input>
    std::tuple<Input, Input> operator()(const Input& lhs, const Input& rhs) {

      return std::make_tuple(lhs, rhs);
    }

  };

}

#endif // SCHED_SHOP_ORDER_CROSSOVER_H
