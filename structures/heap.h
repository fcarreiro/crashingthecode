#ifndef __STRUCTURES_HEAP__
#define __STRUCTURES_HEAP__

#include <cmath>
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

  static void make_heap(std::vector<T>& v, std::size_t last);
  static T pop_top(std::vector<T>& v, std::size_t last);

protected:
  static std::size_t _root();
  static std::size_t _parent(std::size_t i);
  static std::size_t _left(std::size_t i);
  static std::size_t _right(std::size_t i);
  static bool _is_leaf(std::size_t i);

  static std::size_t _bubble_down(std::vector<T>& v, std::size_t i, std::size_t last);
  static std::size_t _bubble_up(std::vector<T>& v, std::size_t i);

protected:
  std::vector<T> _v;
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
  _bubble_up(_v, _v.size() - 1);
}

template<typename T, class Comp>
T Heap<T,Comp>::pop_top() {
  T ret = pop_top(_v, _v.size() - 1);
  _v.pop_back();
  return ret;
}

template<typename T, class Comp>
T Heap<T,Comp>::pop_top(std::vector<T>& v, std::size_t last) {
  T ret = std::move(v[_root()]);
  std::swap(v[_root()], v[last]);

  if(last > 1) {
    _bubble_down(v, _root(), last - 1);
  }

  return ret;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_bubble_down(std::vector<T>& v, std::size_t i, std::size_t last) {
  static Comp _comp;
  while(i < last) {
    const std::size_t left = _left(i);
    const std::size_t right = _right(i);
    std::size_t max_of_three = i;

    if (left <= last && !_comp(v[i], v[left])) {
      max_of_three = left;
    }
    if (right <= last && !_comp(v[max_of_three], v[right])) {
      max_of_three = right;
    }

    if (max_of_three == i) {
      break;
    } else {
      std::swap(v[max_of_three], v[i]);
      i = max_of_three;
    }
  }

  return i;
}

template<typename T, class Comp>
std::size_t Heap<T,Comp>::_bubble_up(std::vector<T>& v, std::size_t i) {
  static Comp _comp;
  while (i > _root()) {
    const std::size_t parent = _parent(i);
    if (!_comp(v[parent], v[i])) {
      std::swap(v[parent], v[i]);
      i = parent;
    } else {
      break;
    }
  }

  return i;
}

template<typename T, class Comp>
void Heap<T,Comp>::make_heap(std::vector<T>& v, std::size_t last) {
  for (std::size_t i = std::ceil(last / 2.0); i > 0; --i) {
    _bubble_down(v, i - 1, last);
  }
}

#endif
