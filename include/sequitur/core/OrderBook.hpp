#include <algorithm>
#include <cstdint>
#include <sequitur/core/Order.hpp>

namespace sequitur {
namespace core {

static constexpr std::size_t MAX_PRICE_TICKS = 1000000;
struct PriceLevel {
  Order *head = nullptr, *tail = nullptr;
  uint32_t volume = 0;
};
class OrderBook {
public:
  PriceLevel book[MAX_PRICE_TICKS + 1];
  uint64_t best_bid = 0, best_ask = MAX_PRICE_TICKS;

  void insert_order(Order *order) {
    uint64_t price = order->price;
    PriceLevel &pl = book[price];
    if (pl.head == nullptr) {
      pl.head = order;
      pl.tail = order;

      order->prev_order = nullptr;
      order->next_order = nullptr;
    } else {
      pl.tail->next_order = order;
      order->prev_order = pl.tail;

      pl.tail = order;
      order->next_order = nullptr;
    }

    pl.volume += order->quantity;

    if (order->side == 0)
      best_bid = std::max(price, best_bid);
    else if (order->side == 1)
      best_ask = std::min(price, best_ask);
  }

  void remove_order(Order *order) {
    PriceLevel &pl = book[order->price];

    if (order->prev_order == nullptr) {
      pl.head = order->next_order;
    } else {
      order->prev_order->next_order = order->next_order;
    }

    if (order->next_order == nullptr) {
      pl.tail = order->prev_order;
    } else {
      order->next_order->prev_order = order->prev_order;
    }

    pl.volume -= order->quantity;
  }
};
} // namespace core
} // namespace sequitur
