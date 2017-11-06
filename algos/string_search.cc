#include <cstddef>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "../test_helpers.h"

// #define LARGE_PRIME 1000000007
#define LARGE_PRIME 2147483647 // largest prime that fits in 32 bits
#define BASE_PRIME 257         // prime related to the aphabet size

inline uint64_t mpow(int base, int expo)
{
  uint64_t ret = 1;
  for (int i=0; i < expo; ++i) {
    ret = (ret * base) % LARGE_PRIME;
  }
  return ret;
}

class RollingHash {
public:
  RollingHash() : _hash(0), _length(0) { }

  void append(unsigned char n) {
    _hash = (_hash * BASE_PRIME + n) % LARGE_PRIME;
    _length++;
  }

  void append(const std::string& s) {
    for (auto& e : s) {
      append(e);
    }
  }

  void pop(unsigned char n) {
    assert(_length > 0);
    uint64_t out = (mpow(BASE_PRIME, _length - 1) * n) % LARGE_PRIME;
    _hash -= out;

    // % is remainder, not modulo
    if (_hash >= LARGE_PRIME) {
      _hash += LARGE_PRIME;
    }

    _length--;
  }

  void pop(const std::string& s) {
    for (auto& e : s) {
      pop(e);
    }
  }

  int length() const {
    return _length;
  }

  uint64_t hash() const {
    return _hash;
  }

private:
  uint64_t _hash;
  int _length;
};

template<class C>
std::vector<int> rabin_karp(const std::string& pattern, const C& stream) {
  RollingHash phash;
  phash.append(pattern);

  RollingHash chash;
  std::deque<char> q;
  std::vector<int> places;
  int i = 0;

  for (const auto& e : stream) {
    chash.append(e);
    q.push_back(e);

    if (q.size() > pattern.size()) {
      const auto& front = q.front();
      chash.pop(front);
      q.pop_front();
    }

    i++;

    // las vegas method
    if (chash.hash() == phash.hash() && std::string(q.begin(), q.end()) == pattern) {
      places.push_back(i - pattern.size());
    }
  }

  return places;
}

int main(int argc, char const *argv[]) {
  TestHelper th;

  {
    RollingHash rh;
    RollingHash f;

    f.append("alongtimeago");
    rh.append("123456789012345678901234567890####****alongtimeago");
    rh.pop("123456789012345678901234567890####****");

    std::cout << "f.hash()  : 0x" << std::hex << f.hash() << std::endl;
    std::cout << "rh.hash() : 0x" << std::hex << rh.hash() << std::endl;
  }

  {
    auto places = rabin_karp("ABABA", "456ABABABABABA123BA");
    std::cout << "Rabin-Karp(ABABA, 456ABABABABABA123BA): ";
    for (const auto& e : places) {
      std::cout << e << ", ";
    }
  }

  th.summary();
  return 0;
}
