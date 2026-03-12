#pragma once

#include <atomic>
#include <vector>

namespace sequitur {
namespace concurrency {

template <typename T, size_t Capacity> class RingBuffer {
private:
  static constexpr size_t Mask = Capacity - 1;
  static_assert((Capacity & (Capacity - 1)) == 0 && Capacity > 0,
                "Capacity must be a power of 2");

  T buffer[Capacity];

  alignas(64) std::atomic<size_t> head{0};
  alignas(64) std::atomic<size_t> tail{0};

  std::atomic<bool> stop_flag{false};

  std::atomic<size_t> push_count{0}, push_rejects{0}, pop_count{0};

public:
  bool push(T item) {
    if (stop_flag.load(std::memory_order_relaxed)) {
      return false;
    }

    size_t t = tail.load(std::memory_order_relaxed);
    size_t h = head.load(std::memory_order_acquire);

    if (((t + 1) & Mask) == (h & Mask)) {
      push_rejects.fetch_add(1, std::memory_order_relaxed);
      return false;
    }

    buffer[t & Mask] = std::move(item);

    tail.store(t + 1, std::memory_order_release);
    push_count.fetch_add(1, std::memory_order_relaxed);
    return true;
  }

  bool pop(T &out_item) {
    size_t h = head.load(std::memory_order_relaxed);
    size_t t = tail.load(std::memory_order_acquire);

    if (h == t)
      return false;

    out_item = std::move(buffer[h & Mask]);
    head.store(h + 1, std::memory_order_release);

    pop_count.fetch_add(1, std::memory_order_relaxed);
    return true;
  }

  size_t pop_batch(std::vector<T> &batch, size_t max) {
    size_t count = 0;
    while (count < max) {
      T item;
      if (!pop(item))
        break;
      batch.push_back(std::move(item));
      count++;
    }
    return count;
  }

  size_t size() const {
    size_t t = tail.load(std::memory_order_relaxed);
    size_t h = head.load(std::memory_order_relaxed);
    return t - h;
  }

  bool empty() const { return size() == 0; }

  void shutdown() { stop_flag.store(true); }
  size_t get_push_count() const {
    return push_count.load(std::memory_order_relaxed);
  }
  size_t get_push_rejects() const {
    return push_rejects.load(std::memory_order_relaxed);
  }
  size_t get_pop_count() const {
    return pop_count.load(std::memory_order_relaxed);
  }
};

} // namespace concurrency
} // namespace sequitur
