#pragma once

#include <cstdint>

namespace sequitur {
namespace core {

struct alignas(64) Order {
  // Identity
  uint64_t id;
  uint32_t instrument_id;
  uint32_t trader_id;

  // Market Data
  uint64_t price;
  uint32_t quantity;
  uint32_t executed_quantity;

  // Intrusive Data Structure
  Order *prev_order;
  Order *next_order;

  // Metadata and Alignment
  uint8_t side;
  uint8_t type;

  uint8_t padding[14];
};

} // namespace core
} // namespace sequitur
