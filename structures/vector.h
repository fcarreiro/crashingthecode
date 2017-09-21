#ifndef __STRUCTURES_VECTOR__
#define __STRUCTURES_VECTOR__

#include <cstddef>
#include <cassert>
#include <algorithm>
#include <memory>
#include <initializer_list>
#include <vector>

template<typename T>
class Vector {
public:
  Vector(); // O(1)
  Vector(const Vector& v); // O(n)
  Vector(Vector&& rvr); // O(1)
  Vector(std::initializer_list<T> l); // O(n)
  Vector& operator=(const Vector& v); // O(n)
  Vector& operator=(Vector&& rvr); // O(1)
  virtual ~Vector(); // O(n)

  std::size_t capacity() const; // O(1)
  std::size_t reserve(std::size_t new_capacity); // O(new_capacity)

  std::size_t size() const; // O(1)
  std::size_t resize(std::size_t new_size); // O(new_size)
  bool empty() const; // O(1)

  T& at(std::size_t i); // O(1)
  const T& at(std::size_t i) const; // O(1)
  T& operator[](std::size_t i); // O(1)
  const T& operator[](std::size_t i) const; // O(1)
  T& back(); // O(1)
  const T& back() const; // O(1)
  std::size_t find(const T& e) const; // O(n)

  void push_back(const T& e); // worst case O(n), amortized O(1)
  void insert(std::size_t i, const T& e); // worst case O(n), amortized O(1)
  T pop_back(); // worst case O(n), amortized O(1)
  T erase(std::size_t i); // worst case O(n), amortized O(1)
  std::size_t remove(const T& e); // worst case O(n), amortized O(1)

  std::vector<T> to_std_vector() const; // O(n)

private:
  T* _arr;
  std::size_t _size;
  std::size_t _capacity;
};

template<typename T>
Vector<T>::Vector() : _arr(new T[1]), _size(0), _capacity(1) {
}

#include <iostream>
template<typename T>
Vector<T>::Vector(std::initializer_list<T> l) : _arr(new T[l.size()]),
_size(l.size()), _capacity(l.size()) {
  // size will be > 0 (and therefore capacity) because of
  // http://en.cppreference.com/w/cpp/language/list_initialization
  // "If the braced-init-list is empty and T is a class type with a
  // default constructor, value-initialization is performed."
  std::move(l.begin(), l.end(), _arr);
}

template<typename T>
Vector<T>::Vector(const Vector& v) : _arr(new T[v._capacity]), _size(v._size), _capacity(v._capacity) {
  std::copy(v._arr, v._arr + v._size, _arr);
}

template<typename T>
Vector<T>::Vector(Vector&& rvr) : _arr(rvr._arr), _size(rvr._size), _capacity(rvr._capacity) {
  rvr._arr = new T[1];
  rvr._size = 0;
  rvr._capacity = 1;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& v) {
  // correctly handles self-assignment
  T* newarr = new T[v._capacity];
  std::copy(v._arr, v._arr + v._size, newarr);
  delete[] _arr;
  _arr = newarr;
  _size = v._size;
  _capacity = v._capacity;
  return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& rvr) {
  // correctly handles self-assignment
  if(this != &rvr) {
    _arr = rvr._arr;
    _size = rvr._size;
    _capacity = rvr._capacity;
    rvr._arr = new T[1];
    rvr._size = 0;
    rvr._capacity = 1;
  }
  return *this;
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
T& Vector<T>::at(std::size_t i) {
  return _arr[i];
}

template<typename T>
const T& Vector<T>::at(std::size_t i) const {
  return _arr[i];
}

template<typename T>
const T& Vector<T>::back() const {
  return _arr[size() - 1];
}

template<typename T>
const T& Vector<T>::operator[](std::size_t i) const {
  return at(i);
}

template<typename T>
T& Vector<T>::operator[](std::size_t i) {
  return at(i);
}

// "resizes the capacity of the vector"
template<typename T>
std::size_t Vector<T>::reserve(std::size_t new_capacity) {
  // don't resize if new capacity cannot hold current elements
  if(new_capacity < size()) {
    return capacity();
  }

  T* newarr = new T[new_capacity];
  std::move(_arr, _arr + size(), newarr);
  delete[] _arr;
  _arr = newarr;
  _capacity = new_capacity;
  return capacity();
}

// resizes the array in such a way that size = capacity = new_size
// if the new size is smaller than the current, the array is truncated,
// if the new size is bigger than the current, new elements are allocated
// using the default constructor of T
template<typename T>
std::size_t Vector<T>::resize(std::size_t new_size) {
  T* newarr = new T[new_size];
  std::move(_arr, _arr + std::min(new_size, _size), newarr);
  delete[] _arr;
  _arr = newarr;
  _size = new_size;
  _capacity = new_size;
  return capacity();
}

template<typename T>
void Vector<T>::insert(std::size_t i, const T& e) {
  assert(i <= size());
  if(size() >= capacity()) {
    reserve(capacity() * 2);
  }

  // shift elements if inserting in the middle,
  // inserting at the end (offset "size") is ok
  // undefined behaviour if i > size or i < 0
  if(i != size()) {
    std::move_backward(
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
T Vector<T>::erase(std::size_t i) {
  T ret = _arr[i];

  // shift elements if removing from the middle,
  // inserting at the end (offset "size") is ok
  // undefined behaviour if i > size or i < 0
  if(i != size() - 1) {
    std::move(
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
    reserve(capacity() / 2);
  }

  return ret;
}

template<typename T>
T Vector<T>::pop_back() {
  return erase(size() - 1);
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
    erase(loc);
  }
  return loc;
}

template<typename T>
std::vector<T> Vector<T>::to_std_vector() const {
  return std::vector<T>(_arr, _arr + size());
}

#endif
