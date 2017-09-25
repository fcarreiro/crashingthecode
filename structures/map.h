#ifndef __STRUCTURES_MAP__
#define __STRUCTURES_MAP__

#include <cstddef>
#include <cassert>
#include <memory>
#include <functional>
#include <list>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <initializer_list>

template<typename K, typename T>
class UnorderedMap {
public:
  virtual ~UnorderedMap() = default;

  virtual T& at(const K& key) = 0;
  virtual const T& at(const K& key) const = 0;

  virtual T& operator[](const K& key) = 0;
  virtual T& operator[](K&& key) = 0;

  virtual void erase(const K& key) = 0;

  virtual std::size_t size() const = 0;
  virtual bool empty() const = 0;

  virtual std::unordered_map<K,T> to_std_unordered_map() const = 0;
};

template<typename K, typename T, class Hash = std::hash<K>>
class ChainedUnorderedMap : public UnorderedMap<K,T> {
public:
  using item_type = std::pair<K,T>;

  ChainedUnorderedMap(const std::size_t bucket_size = 13); // O(bucket_size)
  ChainedUnorderedMap(const ChainedUnorderedMap& other); // O(max(other.size()))
  ChainedUnorderedMap(ChainedUnorderedMap&& rvr); // O(1)
  ChainedUnorderedMap(std::initializer_list<item_type> l); // O(|l|)
  ChainedUnorderedMap& operator=(const ChainedUnorderedMap& other); // O(max(size(), other.size()))
  ChainedUnorderedMap& operator=(ChainedUnorderedMap&& other); // O(size()) to deallocate

  T& at(const K& key); // O(|n|) worst case, O(1) amortized
  const T& at(const K& key) const; // O(|n|) worst case, O(1) amortized

  T& operator[](const K& key); // O(|n|) worst case, O(1) amortized
  T& operator[](K&& key); // O(|n|) worst case, O(1) amortized

  void erase(const K& key); // O(|n|) worst case, O(1) amortized

  std::size_t size() const { return _size; } // O(1)
  bool empty() const { return size() == 0; } // O(1)

  std::size_t bucket_count() const { return _v.size(); } // bucket_count() should be kept O(size)
  float load_factor() const { return bucket_count() > 0 ? (float)size() / bucket_count() : 0.0f; }
  float min_load_factor() const { return _min_load_factor; }
  float max_load_factor() const { return _max_load_factor; }

  std::unordered_map<K,T> to_std_unordered_map() const;

private:
  std::size_t get_bucket_for(const K& key) const;
  void rehash(std::size_t new_size);

  float _min_load_factor = 0.15;
  float _max_load_factor = 0.75;

private:
  // TODO: I'm using std::list and vector because I don't want to (yet)
  // implement iterators for my own types
  std::vector<std::list<item_type>> _v;
  std::size_t _size;
};

template<typename K, typename T, class Hash>
ChainedUnorderedMap<K,T,Hash>::ChainedUnorderedMap(const std::size_t bucket_size) : _size(0) {
  _v.resize(std::max<std::size_t>(bucket_size, 1));
}

template<typename K, typename T, class Hash>
std::size_t ChainedUnorderedMap<K,T,Hash>::get_bucket_for(const K& key) const {
  return Hash()(key) % bucket_count();
}

template<typename K, typename T, class Hash>
ChainedUnorderedMap<K,T,Hash>::ChainedUnorderedMap(const ChainedUnorderedMap& other) : _v(other._v), _size(other._size) {
}

template<typename K, typename T, class Hash>
ChainedUnorderedMap<K,T,Hash>::ChainedUnorderedMap(ChainedUnorderedMap&& rvr) : _v(std::move(rvr._v)), _size(rvr._size) {
  rvr._v.resize(1);
  rvr._size = 0;
}

template<typename K, typename T, class Hash>
ChainedUnorderedMap<K,T,Hash>::ChainedUnorderedMap(std::initializer_list<item_type> l) : _size(0) {
  _v.resize(1);
  for(auto& item : l) {
    operator[](std::move(std::get<0>(item))) = std::move(std::get<1>(item));
  }
}

template<typename K, typename T, class Hash>
ChainedUnorderedMap<K,T,Hash>& ChainedUnorderedMap<K,T,Hash>::operator=(const ChainedUnorderedMap& other) {
  if(this != &other) {
    _v = other._v;
    _size = other._size;
  }
  return *this;
}

template<typename K, typename T, class Hash>
ChainedUnorderedMap<K,T,Hash>& ChainedUnorderedMap<K,T,Hash>::operator=(ChainedUnorderedMap&& other) {
  if(this != &other) {
    _v = std::move(other._v);
    _size = other._size;
    other._v.resize(1);
    other._size = 0;
  }
  return *this;
}

template<typename K, typename T, class Hash>
T& ChainedUnorderedMap<K,T,Hash>::at(const K& key) {
  std::list<item_type>& list = _v[get_bucket_for(key)];
  // pair handling is awful in C++11/14, I hope this becomes mainstream soon
  // https://skebanga.github.io/structured-bindings/
  for(auto& key_value : list) {
    if(key == std::get<0>(key_value)) {
      return std::get<1>(key_value);
    }
  }
  // if we get here, no element is assigned to that key
  throw std::out_of_range("key not present");
}

template<typename K, typename T, class Hash>
T& ChainedUnorderedMap<K,T,Hash>::operator[](const K& key) {
  // use operator[](K&& key)
  // TODO: is it ok to do this? this could make an extra copy
  // and require K to be copy-constructible (which I think we always do)
  // I don't like duplicating code just to add/remove std::move
  return operator[](K(key));
}

template<typename K, typename T, class Hash>
T& ChainedUnorderedMap<K,T,Hash>::operator[](K&& key) {
  std::list<item_type>* list = &_v[get_bucket_for(key)];

  // pair handling is awful in C++11/14, I hope this becomes mainstream soon
  // https://skebanga.github.io/structured-bindings/
  for(auto& key_value : *list) {
    if(key == std::get<0>(key_value)) {
      return std::get<1>(key_value);
    }
  }

  // if we get here, no element is assigned to that key
  // check if we need to rehash
  if(load_factor() > max_load_factor()) {
    rehash(std::max<std::size_t>(bucket_count() * 2, 1));
    list = &_v[get_bucket_for(key)];
  }

  // we create a default one and return it, so that it can be modified
  list->push_back(std::make_pair(std::move(key), T()));
  _size++;
  return std::get<1>(list->back());
}

template<typename K, typename T, class Hash>
const T& ChainedUnorderedMap<K,T,Hash>::at(const K& key) const {
  const std::list<item_type>& list = _v[get_bucket_for(key)];
  // pair handling is awful in C++11/14, I hope this becomes mainstream soon
  // https://skebanga.github.io/structured-bindings/
  for(const auto& key_value : list) {
    if(key == std::get<0>(key_value)) {
      return (const T&)std::get<1>(key_value);
    }
  }
  // if we get here, no element is assigned to that key
  throw std::out_of_range("key not present");
}

template<typename K, typename T, class Hash>
void ChainedUnorderedMap<K,T,Hash>::erase(const K& key) {
  std::list<item_type>& list = _v[get_bucket_for(key)];
  // pair handling is awful in C++11/14, I hope this becomes mainstream soon
  // https://skebanga.github.io/structured-bindings/
  for(auto it = list.begin(); it != list.end(); ++it) {
    if(key == std::get<0>(*it)) {
      list.erase(it);
      _size--;

      // check if we need to rehash
      if(load_factor() < min_load_factor()) {
        rehash(std::max<std::size_t>(bucket_count() / 2, 1));
      }
      return;
    }
  }
  // if we get here, no element is assigned to that key
  // we mimic the behaviour of std::unordered_map::erase and do nothing
}

template<typename K, typename T, class Hash>
void ChainedUnorderedMap<K,T,Hash>::rehash(std::size_t new_size) {
  ChainedUnorderedMap<K,T,Hash> tmp_map(new_size);
  for(auto& bucket : _v) {
    while(!bucket.empty()) {
      auto item = bucket.front();
      tmp_map[std::move(std::get<0>(item))] = std::move(std::get<1>(item));
      bucket.pop_front();
    }
  }
  *this = std::move(tmp_map);
}

template<typename K, typename T, class Hash>
std::unordered_map<K,T> ChainedUnorderedMap<K,T,Hash>::to_std_unordered_map() const {
  std::unordered_map<K,T> m;
  for(const auto& bucket : _v) {
    for(const auto& item : bucket) {
      m[std::get<0>(item)] = std::get<1>(item);
    }
  }
  return m;
}

#endif
