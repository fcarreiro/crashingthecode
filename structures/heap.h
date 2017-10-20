#ifndef __STRUCTURES_HEAP__
#define __STRUCTURES_HEAP__

#include <cstddef>
#include <cassert>
#include <memory>
#include <functional>
#include <vector>

// accepts duplicates
// default is max-heap (parent greater than children)
template<typename T, class Comp = std::greater<T>>
class Heap {
public:
  Heap() = default;
  virtual ~Heap() = default;

  virtual void insert(const T& e);
  virtual void insert(T&& rvr);
  virtual const T& top() const;
  virtual T pop_top();
  virtual std::size_t size() const;
  virtual bool empty() const;

protected:
  static std::size_t _root();
  static std::size_t _parent(std::size_t i);
  static std::size_t _left(std::size_t i);
  static std::size_t _right(std::size_t i);
  virtual std::size_t _last() const;
  static bool _is_leaf(std::size_t i);

  virtual std::size_t _bubble_down(std::size_t i);
  virtual std::size_t _bubble_up(std::size_t i);

protected:
  std::vector<T> _v;
  Comp _comp;
};

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_root() {
  return 0;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_parent(std::size_t i) {
  return ((i + 1) >> 1) - 1;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_left(std::size_t i) {
  return ((i + 1) << 1) - 1;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_right(std::size_t i) {
  return _left(i) + 1;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_last() const {
  return _v.size() - 1;
}

template<typename T, class Comp>
bool Heap<T,Comp>::empty() const {
  return size() == 0;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::size() const {
  return _v.size();
}

template<typename T, class Comp>
const T& Heap<T,Comp>::top() const {
  assert(!empty());
  return _v[_root()];
}

template<typename T, class Comp>
void Heap<T,Comp>::insert(const T& e) {
  insert(std::move(T{e}));
}

template<typename T, class Comp>
void Heap<T,Comp>::insert(T&& rvr) {
  _v.push_back(std::move(rvr));
  _bubble_up(_last());
}

template<typename T, class Comp>
T Heap<T,Comp>::pop_top() {
  T ret = std::move(_v[_root()]);
  std::swap(_v[_root()], _v[_last()]);
  _v.pop_back();

  if(size() > 1) {
    _bubble_down(_root());
  }

  return ret;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_bubble_down(std::size_t i) {
  while(i < _last()) {
    const std::size_t left = _left(i);
    const std::size_t right = _right(i);
    std::size_t max_of_three = i;

    if (left <= _last() && !_comp(_v[i], _v[left])) {
      max_of_three = left;
    }
    if (right <= _last() && !_comp(_v[max_of_three], _v[right])) {
      max_of_three = right;
    }

    if (max_of_three == i) {
      break;
    } else {
      std::swap(_v[max_of_three], _v[i]);
      i = max_of_three;
    }
  }

  return i;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_bubble_up(std::size_t i) {
  while(i > _root()) {
    const std::size_t parent = _parent(i);
    if (!_comp(_v[parent], _v[i])) {
      std::swap(_v[parent], _v[i]);
      i = parent;
    } else {
      break;
    }
  }

  return i;
}

#endif
