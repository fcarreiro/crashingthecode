#ifndef __STRUCTURES_LIST_H__
#define __STRUCTURES_LIST_H__

#include <cstddef>
#include <cassert>
#include <memory>
#include <list>

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

  virtual bool empty() const = 0;
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
  virtual std::list<T> to_std_list() const = 0;
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

  bool empty() const; // O(1)
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
  std::list<T> to_std_list() const; // O(n)

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
    // next pointer of current will be updated in the following cycle
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
bool SinglyLinkedList<T>::empty() const {
  return _head == nullptr;
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
  assert(i >= 0 && i < size());
  return get_node_at(i)->data;
}

template<typename T>
const T& SinglyLinkedList<T>::value_at(std::size_t i) const {
  assert(i >= 0 && i < size());
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
  assert(i >= 0 && i < size());
  Node* current = _head;
  while(i > 0) {
    current = current->next;
    i--;
  }
  return current;
}

template<typename T>
void SinglyLinkedList<T>::clear() {
  while(!empty()) {
    pop_front();
  }
}

template<typename T>
void SinglyLinkedList<T>::insert_at(std::size_t i, const T& v) {
  assert(i >= 0 && i <= size());
  if(i == 0) {
    _head = new Node(v, _head);
  } else {
    // from the assert and first condition we know that here size() > 0
    Node* before_at = get_node_at(i - 1);
    before_at->next = new Node(v, before_at->next);
  }
  _size++;
}

// find out how to de-duplicate code regarding lvalues and rvalue references
template<typename T>
void SinglyLinkedList<T>::insert_at(std::size_t i, T&& v) {
  assert(i >= 0 && i <= size());
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
  assert(i >= 0 && i < size());
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

template<typename T>
std::list<T> SinglyLinkedList<T>::to_std_list() const {
  std::list<T> ret;
  Node* current{_head};
  while(current != nullptr) {
    ret.push_back(current->data);
    current = current->next;
  }
  return ret;
}

// Doubly-linked List implementation (without sentinels)
template<typename T>
class DoublyLinkedList : public List<T> {
public:
  DoublyLinkedList(); // O(1)
  DoublyLinkedList(const DoublyLinkedList& o); // O(n)
  DoublyLinkedList(DoublyLinkedList&& o); // O(1)
  ~DoublyLinkedList(); // O(n)

  DoublyLinkedList& operator=(const DoublyLinkedList& l);

  T& value_at(std::size_t i); // O(i)
  const T& value_at(std::size_t i) const; // O(i)
  T& front(); // O(1)
  T& back(); // O(1)
  const T& front() const; // O(1)
  const T& back() const; // O(1)

  bool empty() const; // O(1)
  std::size_t size() const; // O(1)

  void insert_at(std::size_t i, const T& v); // O(i)
  void insert_at(std::size_t i, T&& v); // O(i)
  void push_front(const T& v); // O(1)
  void push_front(T&& v); // O(1)
  void push_back(const T& v); // O(1)
  void push_back(T&& v); // O(1)

  T remove_at(std::size_t i); // O(i)
  T pop_front(); // O(1)
  T pop_back(); // O(1)

  void clear(); // O(n)
  std::list<T> to_std_list() const; // O(n)

private:
  struct Node {
    Node(const T& v, Node* n = nullptr, Node* p = nullptr);
    Node(T&& prv, Node* n = nullptr, Node* p = nullptr);
    T data;
    Node* prev;
    Node* next;
  };

  Node* _head;
  Node* _tail;
  std::size_t _size;

  Node* get_node_at(std::size_t i) const;
  void push_all(const DoublyLinkedList& o);
};

template<typename T>
DoublyLinkedList<T>::Node::Node(const T& v, Node* n, Node* p) : data(v), prev(p), next(n) {
}

template<typename T>
DoublyLinkedList<T>::Node::Node(T&& prv, Node* n, Node* p) : data(std::move(prv)), prev(p), next(n) {
}

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList() : _head(nullptr), _tail(nullptr), _size(0) {
}

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& o) : _head(nullptr), _tail(nullptr), _size(0) {
  push_all(o);
}

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&& o) : _head(o._head), _tail(o._tail), _size(o._size) {
  o._head = nullptr;
  o._tail = nullptr;
  o._size = 0;
}

template<typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& l) {
  if(this != &l) {
    clear();
    push_all(l);
  }
  return *this;
}

template<typename T>
void DoublyLinkedList<T>::push_all(const DoublyLinkedList& o) {
  Node* oit = o._head;
  Node** current = &_head;
  Node* last = nullptr;
  while(oit != nullptr) {
    *current = new Node(oit->data, nullptr, last);
    last = *current;
    _size++;
    // next pointer of current will be updated in the following cycle
    current = &((*current)->next);
    oit = oit->next;
  }
  _tail = last;
}

template<typename T>
bool DoublyLinkedList<T>::empty() const {
  return _head == nullptr;
}

template<typename T>
std::size_t DoublyLinkedList<T>::size() const {
  return _size;
}

template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
  clear();
}

template<typename T>
T& DoublyLinkedList<T>::value_at(std::size_t i) {
  assert(i >= 0 && i < size());
  return get_node_at(i)->data;
}

template<typename T>
const T& DoublyLinkedList<T>::value_at(std::size_t i) const {
  assert(i >= 0 && i < size());
  return get_node_at(i)->data;
}

template<typename T>
T& DoublyLinkedList<T>::front() {
  return value_at(0);
}

template<typename T>
T& DoublyLinkedList<T>::back() {
  return value_at(size() - 1);
}

template<typename T>
const T& DoublyLinkedList<T>::front() const {
  return value_at(0);
}

template<typename T>
const T& DoublyLinkedList<T>::back() const {
  return value_at(size() - 1);
}

template<typename T>
typename DoublyLinkedList<T>::Node*
DoublyLinkedList<T>::get_node_at(std::size_t i) const {
  assert(i >= 0 && i < size());

  if(i == size() - 1) {
    return _tail;
  }

  // we could start from _head or _tail depending on the shortest distance
  // to i, but it is not really worth it
  Node* current = _head;
  while(i > 0) {
    current = current->next;
    i--;
  }
  return current;
}

template<typename T>
void DoublyLinkedList<T>::clear() {
  while(!empty()) {
    pop_front();
  }
}

template<typename T>
void DoublyLinkedList<T>::insert_at(std::size_t i, const T& v) {
  assert(i >= 0 && i <= size());

  if(size() == 0) {
    // we have to update both head and tail
    _head = new Node(v);
    _tail = _head;
  } else if(i == 0) {
    // at least 1 element and inserting at head, only head needs to be updated
    Node* old_head = _head;
    _head = new Node(v, old_head);
    old_head->prev = _head;
  } else if(i == size()) {
    // at least 1 element and inserting after tail, only tail needs to be updated
    Node* old_tail = _tail;
    _tail = new Node(v, nullptr, old_tail);
    old_tail->next = _tail;
  } else {
    // at least 2 elements and not inserting at head nor after tail
    // element at i exists and has at least one element before
    Node* node_at = get_node_at(i);
    Node* before = node_at->prev;
    before->next = new Node(v, node_at, before);
    node_at->prev = before->next;
  }
  _size++;
}

// find out how to de-duplicate code regarding lvalues and rvalue references
template<typename T>
void DoublyLinkedList<T>::insert_at(std::size_t i, T&& v) {
  assert(i >= 0 && i <= size());

  if(size() == 0) {
    // we have to update both head and tail
    _head = new Node(std::move(v));
    _tail = _head;
  } else if(i == 0) {
    // at least 1 element and inserting at head, only head needs to be updated
    Node* old_head = _head;
    _head = new Node(std::move(v), old_head);
    old_head->prev = _head;
  } else if(i == size()) {
    // at least 1 element and inserting after tail, only tail needs to be updated
    Node* old_tail = _tail;
    _tail = new Node(std::move(v), nullptr, old_tail);
    old_tail->next = _tail;
  } else {
    // at least 2 elements and not inserting at head nor after tail
    // element at i exists and has at least one element before
    Node* node_at = get_node_at(i);
    Node* before = node_at->prev;
    before->next = new Node(std::move(v), node_at, before);
    node_at->prev = before->next;
  }
  _size++;
}

template<typename T>
T DoublyLinkedList<T>::remove_at(std::size_t i) {
  assert(i >= 0 && i < size());

  if(size() == 1) {
    // we have to update both head and tail
    T ret{std::move(_head->data)};
    delete _head;
    _head = nullptr;
    _tail = nullptr;
    _size--;
    return ret;
  } else if(i == 0) {
    // at least 2 elements and removing head, head and next need to be updated
    T ret{std::move(_head->data)};
    Node* new_head = _head->next;
    delete _head;
    new_head->prev = nullptr;
    _head = new_head;
    _size--;
    return ret;
  } else if(i == size() - 1) {
    // at least 2 elements and removing tail, tail and prev need to be updated
    T ret{std::move(_tail->data)};
    Node* new_tail = _tail->prev;
    delete _tail;
    new_tail->next = nullptr;
    _tail = new_tail;
    _size--;
    return ret;
  } else {
    // at least 3 elements, @i exists and has surrounding elements
    Node* node_at = get_node_at(i);
    Node* before = node_at->prev;
    Node* after = node_at->next;
    T ret{std::move(node_at->data)};
    delete node_at;
    before->next = after;
    after->prev = before;
    _size--;
    return ret;
  }
}

template<typename T>
void DoublyLinkedList<T>::push_back(const T& v) {
  return insert_at(size(), v);
}

template<typename T>
void DoublyLinkedList<T>::push_back(T&& v) {
  return insert_at(size(), v);
}

template<typename T>
void DoublyLinkedList<T>::push_front(const T& v) {
  return insert_at(0, v);
}

template<typename T>
void DoublyLinkedList<T>::push_front(T&& v) {
  return insert_at(0, v);
}

template<typename T>
T DoublyLinkedList<T>::pop_front() {
  return remove_at(0);
}

template<typename T>
T DoublyLinkedList<T>::pop_back() {
  return remove_at(size() - 1);
}

template<typename T>
std::list<T> DoublyLinkedList<T>::to_std_list() const {
  std::list<T> ret;
  Node* current{_head};
  while(current != nullptr) {
    ret.push_back(current->data);
    current = current->next;
  }
  return ret;
}

#endif
