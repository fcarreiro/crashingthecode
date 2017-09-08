#ifndef __STRUCTURES_LIST_H__
#define __STRUCTURES_LIST_H__

#include <cstddef>
#include <cassert>
#include <memory>

// List interface
template<typename T>
class List {
public:
  virtual ~List() = default;

  virtual T& value_at(std::size_t i) = 0;
  virtual const T& value_at(std::size_t i) const = 0;
  virtual T& front() = 0;
  virtual T& back() = 0;
  virtual const T& front() const = 0;
  virtual const T& back() const = 0;

  virtual std::size_t size() const = 0;

  virtual void insert_at(std::size_t i, const T& v) = 0;
  virtual void insert_at(std::size_t i, T&& v) = 0;
  virtual void push_front(const T& v) = 0;
  virtual void push_front(T&& v) = 0;
  virtual void push_back(const T& v) = 0;
  virtual void push_back(T&& v) = 0;

  virtual T remove_at(std::size_t i) = 0;
  virtual T pop_front() = 0;
  virtual T pop_back() = 0;

  virtual void clear() = 0;
};

// Singly-linked List implementation (without sentinels)
template<typename T>
class SinglyLinkedList : public List<T> {
public:
  SinglyLinkedList(); // O(1)
  SinglyLinkedList(const SinglyLinkedList& o); // O(n)
  SinglyLinkedList(SinglyLinkedList&& o); // O(1)
  ~SinglyLinkedList(); // O(n)

  T& value_at(std::size_t i); // O(i)
  const T& value_at(std::size_t i) const; // O(i)
  T& front(); // O(1)
  T& back(); // O(n)
  const T& front() const; // O(1)
  const T& back() const; // O(n)

  std::size_t size() const; // O(1)

  void insert_at(std::size_t i, const T& v); // O(i)
  void insert_at(std::size_t i, T&& v); // O(i)
  void push_front(const T& v); // O(1)
  void push_front(T&& v); // O(1)
  void push_back(const T& v); // O(n)
  void push_back(T&& v); // O(n)

  T remove_at(std::size_t i); // O(i)
  T pop_front(); // O(1)
  T pop_back(); // O(n)

  void clear(); // O(n)

private:
  struct Node {
    Node(const T& v, Node* n = nullptr);
    Node(T&& prv, Node* n = nullptr);
    T data;
    Node* next;
  };

  Node* _head;
  std::size_t _size;

  Node* get_node_at(std::size_t i) const;
};

template<typename T>
SinglyLinkedList<T>::Node::Node(const T& v, Node* n) : data(v), next(n) {
}

template<typename T>
SinglyLinkedList<T>::Node::Node(T&& prv, Node* n) : data(std::move(prv)), next(n) {
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList() : _head(nullptr), _size(0) {
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList& o) : _head(nullptr), _size(0) {
  Node* oit = o._head;
  Node** current = &_head;
  while(oit != nullptr) {
    *current = new Node(oit->data);
    _size++;
    current = &((*current)->next);
    oit = oit->next;
  }
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList&& o) : _head(o._head), _size(o._size) {
  o._head = nullptr;
  o._size = 0;
}

template<typename T>
std::size_t SinglyLinkedList<T>::size() const {
  return _size;
}

template<typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
  clear();
}

template<typename T>
T& SinglyLinkedList<T>::value_at(std::size_t i) {
  assert(i < size());
  return get_node_at(i)->data;
}

template<typename T>
const T& SinglyLinkedList<T>::value_at(std::size_t i) const {
  assert(i < size());
  return get_node_at(i)->data;
}

template<typename T>
T& SinglyLinkedList<T>::front() {
  return value_at(0);
}

template<typename T>
T& SinglyLinkedList<T>::back() {
  return value_at(size() - 1);
}

template<typename T>
const T& SinglyLinkedList<T>::front() const {
  return value_at(0);
}

template<typename T>
const T& SinglyLinkedList<T>::back() const {
  return value_at(size() - 1);
}

template<typename T>
typename SinglyLinkedList<T>::Node*
SinglyLinkedList<T>::get_node_at(std::size_t i) const {
  assert(i < size());
  Node* current = _head;
  while(i > 0) {
    current = current->next;
    i--;
  }
  return current;
}

template<typename T>
void SinglyLinkedList<T>::clear() {
  while(_head != nullptr) {
    pop_front();
  }
}

template<typename T>
void SinglyLinkedList<T>::insert_at(std::size_t i, const T& v) {
  assert(size() >= i);
  if(i == 0) {
    _head = new Node(v, _head);
  } else {
    // from the assert and first condition we know that here size() > 0
    Node* before_at = get_node_at(i - 1);
    before_at->next = new Node(v, before_at->next);
  }
  _size++;
}

template<typename T>
void SinglyLinkedList<T>::insert_at(std::size_t i, T&& v) {
  assert(size() >= i);
  if(i == 0) {
    _head = new Node(v, _head);
  } else {
    // from the assert and first condition we know that here size() > 0
    Node* before_at = get_node_at(i - 1);
    before_at->next = new Node(std::move(v), before_at->next);
  }
  _size++;
}

template<typename T>
T SinglyLinkedList<T>::remove_at(std::size_t i) {
  assert(i < size());
  if(i == 0) {
    Node* old_head = _head;
    T ret = std::move(old_head->data);
    _head = _head->next;
    delete old_head;
    _size--;
    return ret;
  } else {
    // there are at least 2 nodes
    Node* before_last = get_node_at(size() - 2);
    T ret = std::move(before_last->next->data);
    delete before_last->next;
    before_last->next = nullptr;
    _size--;
    return ret;
  }
}

template<typename T>
void SinglyLinkedList<T>::push_back(const T& v) {
  return insert_at(size(), v);
}

template<typename T>
void SinglyLinkedList<T>::push_back(T&& v) {
  return insert_at(size(), v);
}

template<typename T>
void SinglyLinkedList<T>::push_front(const T& v) {
  return insert_at(0, v);
}

template<typename T>
void SinglyLinkedList<T>::push_front(T&& v) {
  return insert_at(0, v);
}

template<typename T>
T SinglyLinkedList<T>::pop_front() {
  return remove_at(0);
}

template<typename T>
T SinglyLinkedList<T>::pop_back() {
  return remove_at(size() - 1);
}

#endif
