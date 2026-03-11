#pragma once

#include <memory>
#include <sequitur/core/OrderBook.hpp>
#include <sequitur/memory/ObjectPool.hpp>

using namespace sequitur::memory;

namespace sequitur {
namespace core {
class MatchingEngine {
private:
  ObjectPool<Order> pool;
  std::unique_ptr<OrderBook> book;
  ExecutionQueue exec_q;
  GarbageQueue trash_q;
  uint64_t next_order_id;

public:
  MatchingEngine(size_t pool_capacity)
      : pool(pool_capacity), book(std::make_unique<OrderBook>()),
        next_order_id(1) {}
  void submit_order(uint8_t side, uint64_t price, uint32_t quantity);
};
} // namespace core
} // namespace sequitur
