#include <cstdint>
#include <cstdlib>
#include <new>

namespace sequitur {
namespace memory {

template <typename T> class ObjectPool {
private:
  T *pool;
  uint32_t *free_indices;
  int64_t top;
  size_t capacity;

public:
  ObjectPool(size_t cap) : capacity(cap) {
    size_t total_size = sizeof(T) * cap;
    pool = static_cast<T *>(std::aligned_alloc(64, total_size));
    free_indices = new uint32_t[cap];
    for (int i = 0; i < cap; i++) {
      free_indices[i] = i;
    }

    top = cap - 1;
  }

  T *acquire() {
    if (top < 0)
      return nullptr;
    uint32_t idx = free_indices[top--];

    return new (pool + idx) T();
  }

  void release(T *ptr) {
    if (ptr != nullptr) {
      ptr->~T();
      uint32_t idx = ptr - pool;
      free_indices[++top] = idx;
    }
  }

  ~ObjectPool() {
    std::free(pool);
    delete[] free_indices;
  }
};
} // namespace memory
} // namespace sequitur
