#ifndef __STRUCTURES_VECTOR__
#define __STRUCTURES_VECTOR__

#include <cstddef>
#include <algorithm>
#include <memory>
#include <vector>

template<typename T>
class Vector {
public:
  Vector();
  virtual ~Vector();

  std::size_t size() const;
  std::size_t capacity() const;
  bool empty() const;
  T& at(std::size_t i) const;
  T& back() const;
  T& operator [](std::size_t i) const;
  void push_back(const T& e);
  void insert(std::size_t i, const T& e);
  T pop_back();
  T del(std::size_t i);
  std::size_t find(const T& e) const;
  std::size_t remove(const T& e);
  std::vector<T> to_std_vector() const;

private:
  std::size_t resize(std::size_t new_capacity);

private:
  T* _arr;
  std::size_t _size;
  std::size_t _capacity;
};

template<typename T>
Vector<T>::Vector() : _arr(new T[0]), _size(0), _capacity(0) {
}

template<typename T>
Vector<T>::~Vector() {
  delete[] _arr;
}

template<typename T>
std::size_t Vector<T>::size() const {
  return _size;
}

template<typename T>
std::size_t Vector<T>::capacity() const {
  return _capacity;
}

template<typename T>
bool Vector<T>::empty() const {
  return size() == 0;
}

template<typename T>
T& Vector<T>::at(std::size_t i) const {
  return _arr[i]; // or *(_arr + i)
}

template<typename T>
T& Vector<T>::back() const {
  return _arr[size() - 1];
}

template<typename T>
T& Vector<T>::operator[](std::size_t i) const {
  return at(i);
}

template<typename T>
std::size_t Vector<T>::resize(std::size_t new_capacity) {
  // don't resize if new capacity cannot hold current elements
  if(new_capacity < size()) {
    return capacity();
  }

  T* newarr = new T[new_capacity];
  std::copy(_arr, _arr + size(), newarr);
  delete[] _arr;
  _arr = newarr;
  _capacity = new_capacity;

  return capacity();
}

template<typename T>
void Vector<T>::insert(std::size_t i, const T& e) {
  if(size() >= capacity()) {
    resize(std::max(capacity() * 2, (std::size_t)1));
  }

  // shift elements if inserting in the middle,
  // inserting at the end (offset "size") is ok
  // undefined behaviour if i > size or i < 0
  if(i != size()) {
    std::copy_backward(
      _arr + i,
      _arr + size(),
      _arr + size() + 1
    );
  }

  _arr[i] = e;
  _size++;
}

template<typename T>
void Vector<T>::push_back(const T& e) {
  insert(size(), e);
}

template<typename T>
T Vector<T>::del(std::size_t i) {
  T ret = _arr[i];//std::move(_arr[i]);

  // shift elements if removing from the middle,
  // inserting at the end (offset "size") is ok
  // undefined behaviour if i > size or i < 0
  if(i != size() - 1) {
    std::copy(
      _arr + i + 1,
      _arr + size(),
      _arr + i
    );
  }

  _size--;
  // the shrinking threshold constant (1/4) should be strictly smaller than the
  // shrinking and growing constant (1/2) for the operations
  // to be O(1) amortized
  if(size() < capacity() / 4) {
    resize(capacity() / 2);
  }

  return ret;
}

template<typename T>
T Vector<T>::pop_back() {
  return del(size() - 1);
}

template<typename T>
std::size_t Vector<T>::find(const T& e) const {
  auto ptr = std::find(_arr, _arr + size(), e);
  return ptr - _arr;
}

template<typename T>
std::size_t Vector<T>::remove(const T& e) {
  std::size_t loc = find(e);
  if(loc != size()) {
    del(loc);
  }
  return loc;
}

template<typename T>
std::vector<T> Vector<T>::to_std_vector() const {
  return std::vector<T>(_arr, _arr + size());
}

#endif
