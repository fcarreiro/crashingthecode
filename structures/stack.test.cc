#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stack>
#include "stack.h"
#include "vector.h"
#include "../test_helpers.h"

template<template<typename> class Base>
void test_stack(TestHelper& th) {
  {
    th.message("Default construction");
    Stack<int,Base<int>> s;
    th.tassert();
    th.message("Destruction");
  }
  th.tassert();

  {
    Stack<int,Base<int>> s;
    th.tassert(s.empty(), true, "Initially empty");
    th.tassert(s.size(), (std::size_t)0, "Initially size is 0");

    s.push(7);
    th.tassert(s.empty(), false, "Not empty after pushing element 7");
    th.tassert(s.size(), (std::size_t)1, "Size is 1");
    th.tassert(s.top(), 7, "Top is 7");

    s.push(8);
    th.tassert(s.empty(), false, "Not empty after pushing element 8");
    th.tassert(s.size(), (std::size_t)2, "Size is 2");
    th.tassert(s.top(), 8, "Top is 8");

    {
      th.message("Pop");
      int last = s.pop();
      th.tassert();
      th.tassert(last, 8, "popped == 8");
      th.tassert(s.top(), 7, "Top is 7");
      th.tassert(s.empty(), false, "Not empty");
      th.tassert(s.size(), (std::size_t)1, "Size is 1");
    }

    {
      th.message("Pop");
      int last = s.pop();
      th.tassert();
      th.tassert(last, 7, "popped == 7");
      th.tassert(s.empty(), true, "Empty");
      th.tassert(s.size(), (std::size_t)0, "Size is 0");
    }
  }

  std::srand((unsigned int)std::time(0));
  {
    Stack<int,Base<int>> s;
    std::stack<int> stds;

    th.message("Stress test push");
    for(int i = 0; i < 5000; ++i) {
      th.tassert(s.size(), stds.size(), "Size", true);
      th.tassert(s.empty(), stds.empty(), "Empty", true);
      int r = std::rand();
      s.push(r);
      stds.push(r);
      th.tassert(s.top(), stds.top(), "Top", true);
    }
    th.tassert();

    th.message("Stress test pop");
    for(int i = 0; i < 1000; ++i) {
      th.tassert(s.size(), stds.size(), "Size", true);
      th.tassert(s.empty(), stds.empty(), "Empty", true);
      th.tassert(s.top(), stds.top(), "Top", true);
      s.pop();
      stds.pop();
    }
    th.tassert();
  }
}

int main(int argc, char const *argv[]) {
  TestHelper th;

  std::cout << "[[ List-based Stack ]]" << std::endl << std::endl;
  test_stack<DoublyLinkedList>(th);

  std::cout << std::endl << "[[ Vector-based Stack ]]" << std::endl << std::endl;
  test_stack<Vector>(th);

  th.summary();
  return 0;
}
