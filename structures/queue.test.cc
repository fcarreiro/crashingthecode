#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include "queue.h"
#include "../test_helpers.h"

template<template<typename T> class ConcreteQueue>
void test_queue(TestHelper& th) {
  {
    th.message("Default construction");
    ConcreteQueue<int> q;
    th.tassert();
    th.message("Destruction");
  }
  th.tassert();

  {
    ConcreteQueue<int> q;
    th.tassert(q.empty(), true, "Initially empty");
    th.tassert(q.size(), (std::size_t)0, "Initially size is 0");

    q.enqueue(7);
    th.tassert(q.empty(), false, "Not empty after pushing element 7");
    th.tassert(q.size(), (std::size_t)1, "Size is 1");

    q.enqueue(8);
    th.tassert(q.empty(), false, "Not empty after pushing element 8");
    th.tassert(q.size(), (std::size_t)2, "Size is 2");

    {
      th.message("Pop");
      int last = q.dequeue();
      th.tassert();
      th.tassert(last, 7, "popped == 7");
      th.tassert(q.empty(), false, "Not empty");
      th.tassert(q.size(), (std::size_t)1, "Size is 1");
    }

    {
      th.message("Pop");
      int last = q.dequeue();
      th.tassert();
      th.tassert(last, 8, "popped == 8");
      th.tassert(q.empty(), true, "Empty");
      th.tassert(q.size(), (std::size_t)0, "Size is 0");
    }
  }

  std::srand((unsigned int)std::time(0));
  {
    ConcreteQueue<int> q;
    std::queue<int> stdq;

    th.message("Stress test push");
    for(int i = 0; i < 900; ++i) {
      th.tassert(q.size(), stdq.size(), "Size", true);
      th.tassert(q.empty(), stdq.empty(), "Empty", true);
      int r = std::rand();
      q.enqueue(r);
      stdq.push(r);
      th.tassert(q.to_std_queue() == stdq, true, "Equal queues", true);
    }
    th.tassert();

    th.message("Stress test pop");
    for(int i = 0; i < 500; ++i) {
      th.tassert(q.size(), stdq.size(), "Size", true);
      th.tassert(q.empty(), stdq.empty(), "Empty", true);
      th.tassert(q.to_std_queue() == stdq, true, "Equal queues", true);
      q.dequeue();
      stdq.pop();
    }
    th.tassert();

    th.message("Stress test past end");
    for(int i = 0; i < 300; ++i) {
      th.tassert(q.size(), stdq.size(), "Size", true);
      th.tassert(q.empty(), stdq.empty(), "Empty", true);
      int r = std::rand();
      q.enqueue(r);
      stdq.push(r);
      th.tassert(q.to_std_queue() == stdq, true, "Equal queues", true);
    }
    th.tassert();

    th.message("Stress test pop");
    for(int i = 0; i < 500; ++i) {
      th.tassert(q.size(), stdq.size(), "Size", true);
      th.tassert(q.empty(), stdq.empty(), "Empty", true);
      th.tassert(q.to_std_queue() == stdq, true, "Equal queues", true);
      q.dequeue();
      stdq.pop();
    }
    th.tassert();
  }
}

int main(int argc, char const *argv[]) {
  TestHelper th;

  std::cout << "[[ List-based Queue ]]" << std::endl << std::endl;
  test_queue<ListQueue>(th);

  std::cout << std::endl << "[[ Circular-buffer-based Queue ]]" << std::endl << std::endl;
  test_queue<CircularBufferQueue>(th);

  th.summary();
  return 0;
}
