#ifndef __TEST_HELPERS_
#define __TEST_HELPERS_

#include <iostream>

class TestHelper {
public:
  TestHelper() : _ok(0), _failed(0) { }

  void message(const char *message) const {
    std::cout << message << "... ";
  }

  template<typename T>
  void assert(const T& e1, const T& e2, const char *msg = nullptr) {
    if(msg) {
      message(msg);
    }
    if (e1 == e2) {
      std::cout << tick() << std::endl;
      _ok++;
    }
    else {
      std::cout << cross() << " (" << "expected " << e2
                << " got " << e1 << ")" << std::endl;
      _failed++;
    }
  }

  void assert(bool cond = true, const char *msg = nullptr) {
    assert(cond, true, msg);
  }

  void summary() const {
    std::cout << std::endl << "*** "
              << _ok << " passed " << tick() << " "
              << _failed << " failed " << cross()
              << std::endl;
  }

private:
  const char *tick() const {
    return "\033[32m✔\033[0m";
  }

  const char *cross() const {
    return "\033[31m✖\033[0m";
  }

private:
  unsigned int _ok;
  unsigned int _failed;
};

#endif
