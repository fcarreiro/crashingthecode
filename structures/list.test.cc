#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include "list.h"
#include "../test_helpers.h"

template<template<typename> class L>
void test_list(TestHelper& th) {
  th.message("* Testing on primitive objects");
  {
    L<int> l;
    th.tassert(l.size(), (std::size_t)0, "\nStarting size is 0");

    th.message("Pushing to front [0..4]");
    for(int i = 0; i < 5; ++i) {
      l.push_front(i);
    }
    th.tassert();
    th.tassert(l.back(), 0, "Back is 0");

    th.message("Popping front and checking (5x)");
    for(int i = 0; i < 5; ++i) {
      th.tassert(l.front(), 5 - i - 1, "Front as expected");
      th.tassert(l.size(), (std::size_t)5 - i, "Size as expected");
      th.tassert(l.pop_front(), 5 - i - 1, "Pop returns expected element");
      th.tassert(l.size(), (std::size_t)5 - i - 1, "Size as expected after pop");
    }

    th.message("Push front and change front");
    l.push_front(10);
    l.front() = 20;
    th.tassert(l.front() == 20);
    l.clear();

    th.message("Pushing to back [0..4]");
    for(int i = 0; i < 5; ++i) {
      l.push_back(i);
    }
    th.tassert();

    th.message("Popping front and checking (5x)");
    for(int i = 0; i < 5; ++i) {
      th.tassert(l.front(), i, "Front as expected");
      th.tassert(l.size(), (std::size_t)5 - i, "Size as expected");
      th.tassert(l.pop_front(), i, "Pop returns expected element");
      th.tassert(l.size(), (std::size_t)5 - i - 1, "Size as expected after pop");
    }

    th.message("Pushing to front [0..4]");
    for(int i = 0; i < 5; ++i) {
      l.push_front(i);
    }
    th.tassert();

    th.message("Popping back and checking (5x)");
    for(int i = 0; i < 5; ++i) {
      th.tassert(l.back(), i, "Back as expected");
      th.tassert(l.size(), (std::size_t)5 - i, "Size as expected");
      th.tassert(l.pop_back(), i, "Pop returns expected element");
      th.tassert(l.size(), (std::size_t)5 - i - 1, "Size as expected after pop");
    }
    l.clear();
  }

  th.message("\n* Testing on strings");
  {
    L<std::string> sl;

    th.message("\nPushing to front");
    for(int i = 0; i < 5; ++i) {
      std::stringstream ss;
      ss << "s" << i;
      std::string s(ss.str());
      sl.push_front(s);
    }
    th.tassert();
    {
      auto a = sl.to_std_list();
      auto b = std::list<std::string>{"s4", "s3", "s2", "s1", "s0"};
      th.tassert(a == b, true, "Check complete list");
    }
    sl.clear();

    th.message("Pushing (move) to front");
    for(int i = 0; i < 5; ++i) {
      std::stringstream ss;
      ss << "s" << i;
      std::string s(ss.str());
      sl.push_front(std::move(s));
    }
    th.tassert();
    {
      auto a = sl.to_std_list();
      auto b = std::list<std::string>{"s4", "s3", "s2", "s1", "s0"};
      th.tassert(a == b, true, "Check complete list");
    }
    sl.clear();

    th.message("Pushing (move) to back");
    for(int i = 0; i < 5; ++i) {
      std::stringstream ss;
      ss << "s" << i;
      std::string s(ss.str());
      sl.push_back(std::move(s));
    }
    th.tassert();
    {
      auto a = sl.to_std_list();
      auto b = std::list<std::string>{"s0", "s1", "s2", "s3", "s4"};
      th.tassert(a == b, true, "Check complete list");
    }

    th.message("Copy constructor");
    L<std::string> sl2(sl);
    th.tassert();
    {
      auto a = sl.to_std_list();
      auto b = sl2.to_std_list();
      th.tassert(a == b, true, "Check complete list");
      th.tassert(sl.size(), sl2.size(), "Check sizes");
    }

    th.message("Popping from front");
    for(int i = 0; i < 5; ++i) {
      std::cout << sl2.pop_front() << ",";
    }
    th.tassert();

    th.message("Move constructor");
    auto std_sl = sl.to_std_list();
    L<std::string> sl3(std::move(sl));
    th.tassert();
    {
      auto b = sl3.to_std_list();
      th.tassert(std_sl == b, true, "Check complete list");
      th.tassert(std_sl.size(), sl3.size(), "Check sizes");
    }

    th.message("Popping from front");
    for(int i = 0; i < 5; ++i) {
      std::cout << sl3.pop_front() << ",";
    }
    th.tassert();
    th.tassert(sl3.size(), (std::size_t)0, "Has been emptied");
  }
}

int main(int argc, char const *argv[]) {
  TestHelper th;

  std::cout << "[[ Singly-linked Lists ]]" << std::endl << std::endl;
  test_list<SinglyLinkedList>(th);

  std::cout << "\n[[ Doubly-linked Lists ]]" << std::endl << std::endl;
  test_list<DoublyLinkedList>(th);

  th.summary();

  return 0;
}
