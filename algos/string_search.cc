#include <cmath>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <string>
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

  uint64_t hash() const {
    return _hash;
  }

private:
  uint64_t _hash;
  int _length;
};

int main(int argc, char const *argv[]) {
  TestHelper th;
  std::srand((unsigned int)std::time(0));

  RollingHash rh;
  RollingHash f;

  f.append("alongtimeago");
  rh.append("123456789012345678901234567890####****alongtimeago");
  rh.pop("123456789012345678901234567890####****");

  std::cout << "f.hash()  : 0x" << std::hex << f.hash() << std::endl;
  std::cout << "rh.hash() : 0x" << std::hex << rh.hash() << std::endl;

  th.summary();
  return 0;
}
