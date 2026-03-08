#include <cstdint>

namespace sequitur {
namespace core {
struct Trade {
  uint64_t maker_id;
  uint64_t taker_id;
  uint64_t price;
  uint32_t quantity;
};
} // namespace core
} // namespace sequitur
