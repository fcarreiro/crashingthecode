#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <sstream>
#include <string>
#include "map.h"
#include "../test_helpers.h"

template<typename T>
class Generator {
public:
  virtual T operator()() = 0;
};

class GenInt : public Generator<int> {
public:
  int operator()() { return std::rand(); }
};

class GenString : public Generator<std::string>  {
public:
  std::string operator()() {
    std::ostringstream oss;
    std::size_t size = std::rand() % 100;
    for(std::size_t i = 0; i < size; ++i)
    {
        oss << VALID_CHARS[std::rand() % 62];
    }
    return oss.str();
  }

private:
  static const char constexpr *VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
};

template<typename K, typename T,
  template<typename Ka, typename Ta, class Hash=std::hash<Ka>> class ConcreteMap
>
void test_unordered_map(TestHelper& th, Generator<K>& GenKey, Generator<T>& GenValue) {
  {
    th.message("Default construction");
    ConcreteMap<K, T> m;
    th.tassert();
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Initializer list construction");
    std::initializer_list<typename ConcreteMap<K,T>::item_type> initlist = {
      { GenKey(), GenValue() }, { GenKey(), GenValue() }, { GenKey(), GenValue() }
    };
    ConcreteMap<K, T> m = initlist;
    th.tassert();
    std::unordered_map<K, T> std_map = initlist;
    th.tassert(m.to_std_unordered_map() == std_map, true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Move semantics construction");
    std::initializer_list<typename ConcreteMap<K,T>::item_type> initlist = {
      { GenKey(), GenValue() }, { GenKey(), GenValue() }, { GenKey(), GenValue() }
    };
    ConcreteMap<K, T> m(ConcreteMap<K, T>{initlist});
    th.tassert();
    std::unordered_map<K, T> std_map = initlist;
    th.tassert(m.to_std_unordered_map() == std_map, true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    std::initializer_list<typename ConcreteMap<K,T>::item_type> initlist = {
      { GenKey(), GenValue() }, { GenKey(), GenValue() }, { GenKey(), GenValue() }
    };
    ConcreteMap<K, T> m = initlist;
    th.message("Operator=");
    ConcreteMap<K, T> oeq;
    oeq = m;
    th.tassert();
    th.tassert(m.to_std_unordered_map() == oeq.to_std_unordered_map(), true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Operator= (move)");
    ConcreteMap<K, T> oeq;
    std::initializer_list<typename ConcreteMap<K,T>::item_type> initlist = {
      { GenKey(), GenValue() }, { GenKey(), GenValue() }, { GenKey(), GenValue() }
    };
    oeq = ConcreteMap<K, T>{initlist};
    th.tassert();
    std::unordered_map<K, T> std_map = {initlist};
    th.tassert(oeq.to_std_unordered_map() == std_map, true, "Equal maps");
    th.message("Destruction");
  }
  th.tassert();

  {
    ConcreteMap<K, T> m;
    th.tassert(m.empty(), true, "Initially empty");
    th.tassert(m.size(), (std::size_t)0, "Initially size is 0");

    K key1 = GenKey();
    T val1 = GenValue();
    m[key1] = val1;
    th.tassert(m.empty(), false, "Not empty after setting (K,V)");
    th.tassert(m.size(), (std::size_t)1, "Size is 1");

    K key2 = GenKey();
    T val2 = GenValue();
    m[key2] = val2;
    th.tassert(m.empty(), false, "Not empty after setting (K,V)");
    // TODO: could be 1 if key1=key2
    th.tassert(m.size(), (std::size_t)2, "Size is 2");

    th.tassert(m.at(key1), val1, "m.at(key1) is val1");
    th.tassert(m.at(key2), val2, "m.at(key2) is val2");

    th.message("Resetting key1 -> val3");
    T val3 = GenValue();
    m[key1] = val3;
    th.tassert();
    th.tassert(m.size(), (std::size_t)2, "Size is 2");
    th.tassert(m.at(key1), val3, "m.at(key1) is val3");

    th.message("Erasing m[key1]");
    m.erase(key1);
    th.tassert();
    th.tassert(m.size(), (std::size_t)1, "Size is 1");
    th.message("m.at(key1) throws");
    bool ok = false;
    try {
      m.at(key1);
    } catch (std::out_of_range&) {
      ok = true;
    }
    th.tassert(ok, true);
  }

  {
    ConcreteMap<K, T> m;
    std::unordered_map<K, T> stdm;

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
      K k = GenKey();
      T v = GenValue();
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
      auto key = it->first;
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
      K k = GenKey();
      T v = GenValue();
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
      auto key = it->first;
      ++it;
      stdm.erase(key);
      m.erase(key);
    }
    th.tassert();
  }
}

int main(int argc, char const *argv[]) {
  TestHelper th;
  std::srand((unsigned int)std::time(0));
  auto intGen = GenInt();
  auto stringGen = GenString();

  std::cout << "\n[[ Chained Unordered Map (int, int) ]]" << std::endl << std::endl;
  test_unordered_map<int, int, ChainedUnorderedMap>(th, intGen, intGen);

  std::cout << "\n[[ Chained Unordered Map (string, int) ]]" << std::endl << std::endl;
  test_unordered_map<std::string, int, ChainedUnorderedMap>(th, stringGen, intGen);

  std::cout << "\n[[ Chained Unordered Map (string, string) ]]" << std::endl << std::endl;
  test_unordered_map<std::string, std::string, ChainedUnorderedMap>(th, stringGen, stringGen);

  std::cout << "\n[[ OpenAddress Unordered Map (int, int) ]]" << std::endl << std::endl;
  test_unordered_map<int, int, OpenAddressUnorderedMap>(th, intGen, intGen);

  std::cout << "\n[[ OpenAddress Unordered Map (string, int) ]]" << std::endl << std::endl;
  test_unordered_map<std::string, int, OpenAddressUnorderedMap>(th, stringGen, intGen);

  std::cout << "\n[[ OpenAddress Unordered Map (string, string) ]]" << std::endl << std::endl;
  test_unordered_map<std::string, std::string, OpenAddressUnorderedMap>(th, stringGen, stringGen);

  th.summary();
  return 0;
}
