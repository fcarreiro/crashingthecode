#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include "map.h"
#include "../test_helpers.h"

template<template<typename K, typename T, class Hash=std::hash<K>> class ConcreteMap>
void test_unordered_map(TestHelper& th) {
  {
    th.message("Default construction");
    ConcreteMap<int, int> m;
    th.tassert();
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Initializer list construction");
    ConcreteMap<int, int> m = {{123, 456}, {2, 7}, {20, 30}};
    th.tassert();
    std::unordered_map<int,int> std_map = {{123, 456}, {2, 7}, {20, 30}};
    th.tassert(m.to_std_unordered_map() == std_map, true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Move semantics construction");
    ConcreteMap<int, int> m(ConcreteMap<int, int>{{123, 456}, {2, 7}, {20, 30}});
    th.tassert();
    std::unordered_map<int,int> std_map = {{123, 456}, {2, 7}, {20, 30}};
    th.tassert(m.to_std_unordered_map() == std_map, true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    ConcreteMap<int, int> m = {{123, 456}, {2, 7}, {20, 30}};
    th.message("Operator=");
    ConcreteMap<int, int> oeq;
    oeq = m;
    th.tassert();
    th.tassert(m.to_std_unordered_map() == oeq.to_std_unordered_map(), true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Operator= (move)");
    ConcreteMap<int, int> oeq;
    oeq = ConcreteMap<int, int>{{123, 456}, {2, 7}, {20, 30}};
    th.tassert();
    std::unordered_map<int,int> std_map = {{123, 456}, {2, 7}, {20, 30}};
    th.tassert(oeq.to_std_unordered_map() == std_map, true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    ConcreteMap<int, int> m;
    th.tassert(m.empty(), true, "Initially empty");
    th.tassert(m.size(), (std::size_t)0, "Initially size is 0");

    m[123] = 456;
    th.tassert(m.empty(), false, "Not empty after setting 123 -> 456");
    th.tassert(m.size(), (std::size_t)1, "Size is 1");

    m[2] = 2;
    th.tassert(m.empty(), false, "Not empty after setting 2 -> 2");
    th.tassert(m.size(), (std::size_t)2, "Size is 2");

    th.tassert(m.at(2), 2, "m.at(2) is 2");
    th.tassert(m.at(123), 456, "m.at(123) is 456");

    th.message("Resetting 123 -> 111");
    m[123] = 111;
    th.tassert();
    th.tassert(m.size(), (std::size_t)2, "Size is 2");
    th.tassert(m.at(123), 111, "m.at(123) is 111");

    th.message("Erasing m[2]");
    m.erase(2);
    th.tassert();
    th.tassert(m.size(), (std::size_t)1, "Size is 1");
    th.message("m.at(2) throws");
    bool ok = false;
    try {
      m.at(2);
    } catch (std::out_of_range&) {
      ok = true;
    }
    th.tassert(ok, true);
  }

  std::srand((unsigned int)std::time(0));
  {
    ConcreteMap<int, int> m;
    std::unordered_map<int, int> stdm;

    th.message("Stress test insert");
    for(int i = 0; i < 900; ++i) {
      th.tassert(m.size(), stdm.size(), "Size", true);
      th.tassert(m.empty(), stdm.empty(), "Empty", true);
      if(i % 20 == 0) {
        std::cout << std::endl
                  << "[+] Size: " << m.size()
                  << ", Buckets: " << m.bucket_count()
                  << ", Load factor: " << m.load_factor();
      }
      int k = std::rand();
      int v = std::rand();
      m[k] = v;
      stdm[k] = v;
      th.tassert(m.to_std_unordered_map() == stdm, true, "Equal maps", true);
    }
    th.tassert();

    th.message("\nStress test erase");
    auto it = stdm.begin();
    for(int i = 0; i < 750; ++i) {
      th.tassert(m.size(), stdm.size(), "Size", true);
      th.tassert(m.empty(), stdm.empty(), "Empty", true);
      th.tassert(m.to_std_unordered_map() == stdm, true, "Equal maps", true);
      if(i % 20 == 0) {
        std::cout << std::endl
                  << "[-] Size: " << m.size()
                  << ", Buckets: " << m.bucket_count()
                  << ", Load factor: " << m.load_factor();
      }
      const int& key = it->first;
      ++it;
      stdm.erase(key);
      m.erase(key);
    }
    th.tassert();

    th.message("\nStress test insert after erase");
    for(int i = 0; i < 300; ++i) {
      th.tassert(m.size(), stdm.size(), "Size", true);
      th.tassert(m.empty(), stdm.empty(), "Empty", true);
      if(i % 20 == 0) {
        std::cout << std::endl
                  << "[+] Size: " << m.size()
                  << ", Buckets: " << m.bucket_count()
                  << ", Load factor: " << m.load_factor();
      }
      int k = std::rand();
      int v = std::rand();
      m[k] = v;
      stdm[k] = v;
      th.tassert(m.to_std_unordered_map() == stdm, true, "Equal maps", true);
    }
    th.tassert();

    th.message("\nStress test erase");
    it = stdm.begin();
    for(int i = 0; i < 400; ++i) {
      th.tassert(m.size(), stdm.size(), "Size", true);
      th.tassert(m.empty(), stdm.empty(), "Empty", true);
      th.tassert(m.to_std_unordered_map() == stdm, true, "Equal maps", true);
      if(i % 20 == 0) {
        std::cout << std::endl
                  << "[-] Size: " << m.size()
                  << ", Buckets: " << m.bucket_count()
                  << ", Load factor: " << m.load_factor();
      }
      const int& key = it->first;
      ++it;
      stdm.erase(key);
      m.erase(key);
    }
    th.tassert();
  }
}

int main(int argc, char const *argv[]) {
  TestHelper th;

  std::cout << "[[ Chained Unordered Map ]]" << std::endl << std::endl;
  test_unordered_map<ChainedUnorderedMap>(th);

  th.summary();
  return 0;
}
