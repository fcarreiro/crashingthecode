#ifndef __TEST_HELPERS__
#define __TEST_HELPERS__

#include <iostream>

class TestHelper {
public:
  TestHelper() : _ok(0), _failed(0) { }

  void message(const char *message) const {
    std::cout << message << "... ";
  }

  template<typename T>
  void tassert(const T& e1, const T& e2, const char *msg = nullptr, bool silent = false) {
    if (e1 == e2) {
      if(!silent) {
        if(msg) {
          std::cout << msg << "... ";
        }
        std::cout << tick() << std::endl;
      }
      _ok++;
    }
    else {
      if(msg) {
        std::cout << msg << "... ";
      }
      std::cout << cross()
                << " (" << "expected " << e1
                << " got " << e2 << ")" << std::endl;
      _failed++;
    }
  }

  void tassert(bool cond = true, const char *msg = nullptr) {
    tassert(cond, true, msg);
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
