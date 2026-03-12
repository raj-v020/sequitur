#pragma once

#include <memory>
#include <sequitur/core/OrderBook.hpp>
#include <sequitur/memory/ObjectPool.hpp>

namespace sequitur {
namespace core {
class MatchingEngine {
private:
  memory::ObjectPool<Order> pool;
  std::unique_ptr<OrderBook> book;
  ExecutionQueue exec_q;
  GarbageQueue trash_q;
  uint64_t next_order_id;

  uint64_t total_orders = 0;

public:
  MatchingEngine(size_t pool_capacity)
      : pool(pool_capacity), book(std::make_unique<OrderBook>()),
        next_order_id(1) {}
  void submit_order(uint8_t side, uint64_t price, uint32_t quantity);
  uint64_t get_total_orders() const { return total_orders; }
};
} // namespace core
} // namespace sequitur
