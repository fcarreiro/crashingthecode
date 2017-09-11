#ifndef __STRUCTURES_STACK_H__
#define __STRUCTURES_STACK_H__

#include <cstddef>
#include <memory>
#include "list.h"

template<typename T, class Base=DoublyLinkedList<T>>
class Stack {
public:
  Stack() = default;
  Stack(const Stack& s) : _base(s._base) { }
  Stack(Stack& rvr) : _base(std::move(rvr._base)) { }

  void push(const T& v) {
    return _base.push_back(v);
  }

  void push(T&& rvr) {
    return _base.push_back(std::move(rvr));
  }

  T pop() {
    return _base.pop_back();
  }

  void clear() {
    return _base.clear();
  }

  T top() const {
    return _base.back();
  }

  bool empty() const {
    return _base.empty();
  }

  std::size_t size() const {
    return _base.size();
  }

private:
  Base _base;
};

#endif
