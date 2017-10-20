#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "heap.h"
#include "../test_helpers.h"

int main(int argc, char const *argv[]) {
  TestHelper th;

  {
    th.message("Default construction");
    Heap<int> h;
    th.tassert();
    th.message("Destruction");
  }
  th.tassert();

  {
    Heap<int> h;
    th.message("Inserting 6 elements");
    h.insert(5);
    h.insert(2);
    h.insert(10);
    h.insert(1);
    h.insert(-5);
    h.insert(28);
    th.tassert();
    th.message("Destruction with elements");
  }
  th.tassert();

  {
    Heap<int> h;
    std::vector<int> v;
    th.tassert(h.size(), (std::size_t)0, "Size is 0 before inserting");
    h.insert(5);
    h.insert(2);
    h.insert(10);
    h.insert(1);
    h.insert(-5);
    h.insert(28);
    th.tassert(h.size(), (std::size_t)6, "Size is 6 after inserting 6 items");
    th.message("Exhausting top elements returns descending sort");
    while (!h.empty()) {
      v.push_back(h.pop_top());
    }
    for(auto i : v) {
      std::cout << i << " ";
    }
    th.tassert(v == std::vector<int>{28,10,5,2,1,-5});
  }

  std::srand((unsigned int)std::time(0));
  {
    Heap<int> h;
    std::vector<int> v, poppedv;
    th.message("Stress test insert, pop, sorted");
    for(int i = 0; i < 50000; ++i) {
      int r = std::rand();
      v.push_back(r);
      h.insert(r);
    }
    while (!h.empty()) {
      poppedv.push_back(h.pop_top());
    }
    std::sort(v.begin(), v.end(), std::greater<int>());
    th.tassert(v == poppedv);
  }

  th.summary();
  return 0;
}
