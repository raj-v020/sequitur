#include <cstddef>
#include <cstdint>
#include <iterator>

namespace sequitur {
namespace core {

template <typename T, std::size_t Capacity> class FixedQueue {
private:
  T data[Capacity];
  uint32_t count = 0;

public:
  void push(const T &item) { data[count++] = item; }

  void clear() { count = 0; }
  uint32_t size() const { return count; }
  const T &operator[](uint32_t index) const { return data[index]; }

  const T *begin() const { return data; }
  const T *end() const { return data + count; }
};
} // namespace core
} // namespace sequitur
