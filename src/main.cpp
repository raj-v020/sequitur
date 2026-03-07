#include <iostream>
#include <sequitur/core/Order.hpp>
#include <sequitur/memory/ObjectPool.hpp>
#include <sequitur/utils/Timer.hpp>

int main() {
  sequitur::memory::ObjectPool<sequitur::core::Order> pool(10'000'000);
  {
    sequitur::utils::Timer t;
    for (size_t i = 0; i < 10'000'000; i++) {
      pool.acquire();
    }
  }

  std::cout << "Sequitur Exchange Engine: Online" << std::endl;
  return 0;
}
