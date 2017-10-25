#ifndef __STRUCTURES_TREE__
#define __STRUCTURES_TREE__

#include <cstddef>
#include <cassert>
#include <memory>
#include <list>
#include <queue>
#include <stack>
#include <functional>
#include <iostream>

template<typename T>
class BinarySearchTree {
public:
  BinarySearchTree();
  ~BinarySearchTree(); // O(n)
  // TODO: copy and move constructors
  // TODO: assignment operator

  std::size_t size() const; // O(1)
  std::size_t height() const; // single node's height is 1
  bool empty() const; // O(1)

  bool find(const T& o) const;
  void print() const;

  void insert(const T& d);
  void insert(T&& prv);

  bool remove(const T& d);
  void clear(); // O(n)

  void visit_bfs(std::function<void(const T&)> f) const;
  void visit_preorder(std::function<void(const T&)> f) const;
  void visit_inorder(std::function<void(const T&)> f) const;
  void visit_postorder(std::function<void(const T&)> f) const;

private:
  struct Node {
    Node(const T& d, Node* l = nullptr, Node* r = nullptr);
    Node(T&& rvr, Node* l = nullptr, Node* r = nullptr);

    T data;
    Node* left;
    Node* right;
  };

  Node** _get_node_for(const T& d) const;
  static Node** _inorder_successor_of(Node *n);

  static void _remove(Node** nptr);
  static std::size_t _node_height(Node* n);

  static void _visit_nodes_bfs(Node* n, std::function<void(Node*)> f);
  static void _visit_nodes_preorder(Node* n, std::function<void(Node*)> f);
  static void _visit_nodes_inorder(Node* n, std::function<void(Node*)> f);
  static void _visit_nodes_postorder(Node* n, std::function<void(Node*)> f);

private:
  Node* _root;
  std::size_t _size;
};

template<typename T>
BinarySearchTree<T>::Node::Node(const T& d, Node* l, Node* r) : data(d), left(l), right(r) {
}

template<typename T>
BinarySearchTree<T>::Node::Node(T&& rvr, Node* l, Node* r) : data(std::move(rvr)), left(l), right(r) {
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree() : _root(nullptr), _size(0) {
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree() {
  clear();
}

template<typename T>
typename BinarySearchTree<T>::Node** BinarySearchTree<T>::_get_node_for(const T& d) const {
  // this will return either
  // (a) pointer to (a pointer to) node holding data "d"
  // (b) pointer to (a pointer to) node where "d" would be found if inserted
  auto ptr = &_root; // Node* const* ptr
  auto n = _root; // const Node* n

  while (n != nullptr && n->data != d) {
    if(d < n->data) {
      ptr = &(n->left);
      n = n->left;
    } else if (d > n->data) {
      ptr = &(n->right);
      n = n->right;
    }
  }

  return (Node**)ptr;
}

template<typename T>
std::size_t BinarySearchTree<T>::height() const {
  return _node_height(_root);
}

template<typename T>
std::size_t BinarySearchTree<T>::size() const {
  return _size;
}

template<typename T>
std::size_t BinarySearchTree<T>::_node_height(Node* n) {
  return (n == nullptr ? 0 : 1 + std::max(_node_height(n->left), _node_height(n->right)));
}

template<typename T>
typename BinarySearchTree<T>::Node** BinarySearchTree<T>::_inorder_successor_of(Node *n) {
  assert(n != nullptr && n->right != nullptr);
  Node** cptr = &(n->right);
  while ((*cptr)->left != nullptr) {
    cptr = &((*cptr)->left);
  }
  return cptr;
}

template<typename T>
void BinarySearchTree<T>::clear() {
  // delete nodes in post-order
  // left, right, self
  _visit_nodes_postorder(_root, [](Node* n) {
    delete n;
  });

  _size = 0;
  _root = nullptr;
}

template<typename T>
bool BinarySearchTree<T>::empty() const {
  return size() == 0;
}

template<typename T>
void BinarySearchTree<T>::print() const {
  visit_inorder([](const T& e) { std::cout << e << " "; });
}

template<typename T>
bool BinarySearchTree<T>::find(const T& o) const {
  Node** nptr = _get_node_for(o);
  return (*nptr != nullptr);
}

template<typename T>
void BinarySearchTree<T>::insert(T&& d) {
  Node** nptr = _get_node_for(d);
  Node* n = *nptr;

  // if n != nullptr this means that n->data == d and hence we are done
  // because we don't allow duplicates, otherwise
  // nptr points to the child of a leaf (&leaf->left) or (&leaf->right)
  if (n == nullptr) {
    *nptr = new Node(std::move(d));
    _size++;
  }
}

template<typename T>
void BinarySearchTree<T>::insert(const T& d) {
  insert(std::move(T{d}));
}

template<typename T>
bool BinarySearchTree<T>::remove(const T& d) {
  Node** nptr = _get_node_for(d);
  Node* n = *nptr;

  if (n != nullptr && n->data == d) {
    _remove(nptr);
    _size--;
    return true;
  }

  return false;
}

template<typename T>
void BinarySearchTree<T>::_remove(Node** nptr) {
  Node* n = *nptr;

  if (n->left == nullptr && n->right == nullptr) {
    // leaf case
    delete n;
    *nptr = nullptr;
  } else if (n->left != nullptr && n->right == nullptr) {
    // one child case (left)
    *nptr = n->left;
    delete n;
  } else if (n->right != nullptr && n->left == nullptr) {
    // one child case (right)
    *nptr = n->right;
    delete n;
  } else {
    // two children case
    // suc will have at most one (right) child, therefore the removal
    // will finish in one more call to _remove
    Node** suc = _inorder_successor_of(n);
    n->data = std::move((*suc)->data);
    _remove(suc);
  }
}

template<typename T>
void BinarySearchTree<T>::_visit_nodes_bfs(Node* n, std::function<void(Node*)> f) {
  if (n != nullptr) {
    std::queue<Node*> q;
    q.push(n);
    while (!q.empty()) {
      auto current = q.front();
      q.pop();
      if (current != nullptr) {
        f(current);
        q.push(current->left);
        q.push(current->right);
      }
    }
  }
}

template<typename T>
void BinarySearchTree<T>::_visit_nodes_inorder(Node* n, std::function<void(Node*)> f) {
  if (n != nullptr) {
    _visit_nodes_inorder(n->left, f);
    f(n);
    _visit_nodes_inorder(n->right, f);
  }
}

template<typename T>
void BinarySearchTree<T>::_visit_nodes_preorder(Node* n, std::function<void(Node*)> f) {
  if (n != nullptr) {
    f(n);
    _visit_nodes_preorder(n->left, f);
    _visit_nodes_preorder(n->right, f);
  }
}

template<typename T>
void BinarySearchTree<T>::_visit_nodes_postorder(Node* n, std::function<void(Node*)> f) {
  if (n != nullptr) {
    _visit_nodes_postorder(n->left, f);
    _visit_nodes_postorder(n->right, f);
    f(n);
  }
}

template<typename T>
void BinarySearchTree<T>::visit_bfs(std::function<void(const T&)> f) const {
  _visit_nodes_bfs(_root, [f](Node* n) { f(n->data); });
}

template<typename T>
void BinarySearchTree<T>::visit_preorder(std::function<void(const T&)> f) const {
  _visit_nodes_preorder(_root, [f](Node* n) { f(n->data); });
}

template<typename T>
void BinarySearchTree<T>::visit_inorder(std::function<void(const T&)> f) const {
  _visit_nodes_inorder(_root, [f](Node* n) { f(n->data); });
}

template<typename T>
void BinarySearchTree<T>::visit_postorder(std::function<void(const T&)> f) const {
  _visit_nodes_postorder(_root, [f](Node* n) { f(n->data); });
}

#endif
