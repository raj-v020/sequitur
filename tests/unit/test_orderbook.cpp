#include <gtest/gtest.h>
#include <memory>
#include <sequitur/core/OrderBook.hpp>
#include <sequitur/memory/ObjectPool.hpp>

using namespace sequitur::core;
using namespace sequitur::memory;

TEST(OrderBookTest, PassiveMaker) {

  ObjectPool<Order> op(100);
  std::unique_ptr<OrderBook> ob = std::make_unique<OrderBook>();
  ExecutionQueue exec_q;
  GarbageQueue trash_q;

  Order *order = op.acquire();
  order->id = 1;
  order->side = 0;
  order->price = 100;
  order->quantity = 50;

  ob->match_order(order, exec_q, trash_q);

  EXPECT_EQ(exec_q.size(), 0);
  EXPECT_EQ(trash_q.size(), 0);
  EXPECT_EQ(ob->best_bid, 100);
  EXPECT_EQ(ob->book[100].head, order);
  EXPECT_EQ(ob->book[100].volume, 50);
}

TEST(OrderBookTest, PerfectAnnihilation) {

  ObjectPool<Order> op(100);
  std::unique_ptr<OrderBook> ob = std::make_unique<OrderBook>();
  ExecutionQueue exec_q;
  GarbageQueue trash_q;

  Order *maker_order = op.acquire();

  maker_order->id = 1;
  maker_order->side = 1;
  maker_order->price = 150;
  maker_order->quantity = 100;

  ob->match_order(maker_order, exec_q, trash_q);

  Order *taker_order = op.acquire();

  taker_order->id = 2;
  taker_order->side = 0;
  taker_order->price = 150;
  taker_order->quantity = 100;

  ob->match_order(taker_order, exec_q, trash_q);

  EXPECT_EQ(exec_q.size(), 1);
  EXPECT_EQ(trash_q.size(), 1);
  EXPECT_EQ(exec_q[0].quantity, 100);
  EXPECT_EQ(trash_q[0], maker_order);
  EXPECT_EQ(ob->book[150].head, nullptr);
  EXPECT_EQ(ob->book[150].volume, 0);
}

TEST(OrderBookTest, PartialFill) {

  ObjectPool<Order> op(100);
  std::unique_ptr<OrderBook> ob = std::make_unique<OrderBook>();
  ExecutionQueue exec_q;
  GarbageQueue trash_q;

  Order *maker_order = op.acquire();

  maker_order->id = 1;
  maker_order->side = 1;
  maker_order->price = 150;
  maker_order->quantity = 100;

  ob->match_order(maker_order, exec_q, trash_q);

  Order *taker_order = op.acquire();

  taker_order->id = 2;
  taker_order->side = 0;
  taker_order->price = 150;
  taker_order->quantity = 40;

  ob->match_order(taker_order, exec_q, trash_q);

  EXPECT_EQ(exec_q.size(), 1);
  EXPECT_EQ(trash_q.size(), 0);
  EXPECT_EQ(exec_q[0].quantity, 40);
  EXPECT_EQ(maker_order->quantity, 60);
  EXPECT_EQ(ob->book[150].head, maker_order);
  EXPECT_EQ(ob->book[150].volume, 60);
}

TEST(OrderBookTest, PriceSweeper) {

  ObjectPool<Order> op(100);
  std::unique_ptr<OrderBook> ob = std::make_unique<OrderBook>();
  ExecutionQueue exec_q;
  GarbageQueue trash_q;

  Order *maker_order1 = op.acquire();
  Order *maker_order2 = op.acquire();
  Order *maker_order3 = op.acquire();

  maker_order1->id = 1;
  maker_order1->side = 1;
  maker_order1->price = 150;
  maker_order1->quantity = 100;

  maker_order2->id = 2;
  maker_order2->side = 1;
  maker_order2->price = 151;
  maker_order2->quantity = 100;

  maker_order3->id = 3;
  maker_order3->side = 1;
  maker_order3->price = 152;
  maker_order3->quantity = 100;

  ob->match_order(maker_order1, exec_q, trash_q);
  ob->match_order(maker_order2, exec_q, trash_q);
  ob->match_order(maker_order3, exec_q, trash_q);

  Order *taker_order = op.acquire();

  taker_order->id = 4;
  taker_order->side = 0;
  taker_order->price = 155;
  taker_order->quantity = 250;

  ob->match_order(taker_order, exec_q, trash_q);

  EXPECT_EQ(exec_q.size(), 3);
  EXPECT_EQ(trash_q.size(), 2);
  EXPECT_EQ(exec_q[0].quantity, 100);
  EXPECT_EQ(exec_q[1].quantity, 100);
  EXPECT_EQ(exec_q[2].quantity, 50);
  EXPECT_EQ(maker_order3->quantity, 50);
  EXPECT_EQ(ob->best_ask, 152);
}
