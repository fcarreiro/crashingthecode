#ifndef __STRUCTURES_QUEUE__
#define __STRUCTURES_QUEUE__

#include <cstddef>
#include <cassert>
#include <algorithm>
#include <queue>
#include "list.h"

// Queue interface
template<typename T>
class Queue {
public:
  virtual ~Queue() = default;

  virtual void enqueue(const T& v) = 0;
  virtual void enqueue(T&& rvr) = 0;
  virtual T dequeue() = 0;
  virtual bool empty() const = 0;
  virtual std::size_t size() const = 0;

  virtual std::queue<T> to_std_queue() const = 0;
};

// Unbounded queue based on singly-linked list
template<typename T>
class ListQueue : Queue<T> {
public:
  void enqueue(const T& v) { // O(1)
    _list.push_back(v);
  }

  void enqueue(T&& rvr) { // O(1)
    _list.push_back(std::move(rvr));
  }

  T dequeue() { // O(1)
    return _list.pop_front();
  }

  bool empty() const { // O(1)
    return _list.empty();
  }

  std::size_t size() const { // O(1)
    return _list.size();
  }

  std::queue<T> to_std_queue() const { // O(n)
    // standard queue is based on deque which has O(1) push_back
    std::queue<T> q;
    for(auto e : _list.to_std_list()) {
      q.push(e);
    }
    return q;
  }

private:
  DoublyLinkedList<T> _list;
};

// Bounded queue based on circular buffer
// NOTE: A possible improvement would be to implement the buffer using
// some kind of vector and resize and reposition the items when the
// capacity is met
template<typename T>
class CircularBufferQueue : Queue<T> {
public:
  CircularBufferQueue(std::size_t capacity = 1000) :
  _buffer(new T[std::max<std::size_t>(capacity, 1)]),
  _capacity(std::max<std::size_t>(capacity, 1)),
  _size(0), _head(0), _tail(0) {
  }

  ~CircularBufferQueue() {
    delete[] _buffer;
  }

  // CircularBufferQueue(const CircularBufferQueue& q) :
  // _buffer(new T[q._capacity]{q._buffer}), _capacity(q._capacity),
  // _size(q._size), _head(q._head), _tail(q._tail) {
  //   aaaaa
  // }

  void enqueue(const T& v) { // O(1)
    // we need to always have at least one free slot
    assert(size() + 1 < capacity());
    _buffer[_tail] = v;
    _tail = (_tail + 1) % capacity();
    _size++;
  }

  void enqueue(T&& rvr) { // O(1)
    // we need to always have at least one free slot
    assert(size() + 1 < capacity());
    _buffer[_tail] = std::move(rvr);
    _tail = (_tail + 1) % capacity();
    _size++;
  }

  T dequeue() { // O(1)
    assert(!empty());
    size_t old_head = _head;
    _head = (_head + 1) % capacity();
    _size--;
    return _buffer[old_head];
  }

  bool empty() const { // O(1)
    return size() == 0;
  }

  bool full() const { // O(1)
    return size() + 1 >= capacity();
  }

  std::size_t size() const { // O(1)
    return _size;
  }

  std::size_t capacity() const { // O(1)
    return _capacity;
  }

  std::queue<T> to_std_queue() const { // O(n)
    std::queue<T> q;
    for(std::size_t it = _head; it != _tail; it = (it + 1) % capacity()) {
      q.push(_buffer[it]);
    }
    return q;
  }

private:
  T* _buffer;
  std::size_t _capacity;
  // size could be computed using tail and head but it's more readable this way
  // (and faster, for that matter)
  std::size_t _size;
  // head points to the first element (if not empty)
  std::size_t _head;
  // tail points to the element after the last
  std::size_t _tail;
};

#endif
