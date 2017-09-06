#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "vector.h"
#include "../test_helpers.h"

int main(int argc, char const *argv[]) {
  TestHelper th;

  {
    th.message("Default construction");
    Vector<int> v;
    th.assert();
    th.message("Destruction");
  }
  th.assert();

  {
    Vector<int> v;
    th.assert(v.capacity(), (std::size_t)0, "Initial capacity is 0");

    v.push(7);
    th.assert(v.capacity(), (std::size_t)1, "Capacity after one push is 1");
    th.assert(v.size(), (std::size_t)1, "Size after one push is 1");

    v.push(8);
    th.assert(v.capacity(), (std::size_t)2, "Capacity after two pushes is 2");
    th.assert(v.size(), (std::size_t)2, "Size after two pushes is 2");

    v.push(9);
    th.assert(v.capacity(), (std::size_t)4, "Capacity after three pushes is 4");
    th.assert(v.size(), (std::size_t)3, "Size after three pushes is 3");

    th.assert(v.at(0), 7, "v.at(0) == 7");
    th.assert(v[0], 7, "v[0] == 7");
    th.assert(v.at(1), 8, "v.at(1) == 8");
    th.assert(v[1], 8, "v[1] == 8");
    th.assert(v.at(2), 9, "v.at(2) == 9");
    th.assert(v[2], 9, "v[2] == 9");

    th.message("Insert 1 at offset 0");
    v.insert(0, 1);
    th.assert();
    th.assert(v[0], 1, "v[0] == 1");
    th.assert(v[1], 7, "v[1] == 7");
    th.assert(v[2], 8, "v[2] == 8");
    th.assert(v[3], 9, "v[3] == 9");
    th.assert(v.size(), (std::size_t)4, "Size is 4");

    th.message("Insert 5 at offset 2");
    v.insert(2, 5);
    th.assert();
    th.assert(v[0], 1, "v[0] == 1");
    th.assert(v[1], 7, "v[1] == 7");
    th.assert(v[2], 5, "v[2] == 5");
    th.assert(v[3], 8, "v[3] == 8");
    th.assert(v[4], 9, "v[4] == 9");
    th.assert(v.size(), (std::size_t)5, "Size is 5");

    th.message("Pop");
    int last = v.pop();
    th.assert();
    th.assert(last, 9, "popped == 9");
    th.assert(v[0], 1, "v[0] == 1");
    th.assert(v[1], 7, "v[1] == 7");
    th.assert(v[2], 5, "v[2] == 5");
    th.assert(v[3], 8, "v[3] == 8");
    th.assert(v.size(), (std::size_t)4, "Size is 4");
    th.assert(v.capacity(), (std::size_t)8, "Capacity is 8");

    th.assert(v.find(5), (std::size_t)2, "Element 5 is found at location 2");
    th.assert(v.find(8), (std::size_t)3, "Element 8 is found at location 3");

    th.message("Remove 5");
    std::size_t five_idx = v.remove(5);
    th.assert();
    th.assert(five_idx, (std::size_t)2, "5 was at index 2");
    th.assert(v[0], 1, "v[0] == 1");
    th.assert(v[1], 7, "v[1] == 7");
    th.assert(v[2], 8, "v[2] == 8");
    th.assert(v.size(), (std::size_t)3, "Size is 3");
    th.assert(v.capacity(), (std::size_t)4, "Capacity is 4");

    th.message("Delete index 0");
    int elem_at_zero = v.del(0);
    th.assert();
    th.assert(elem_at_zero, 1, "Element at index 0 was 1");
    th.assert(v[0], 7, "v[0] == 7");
    th.assert(v[1], 8, "v[1] == 8");
    th.assert(v.size(), (std::size_t)2, "Size is 2");
    th.assert(v.capacity(), (std::size_t)4, "Capacity is 4");

    th.message("Change element at 0 to 27");
    v[0] = 27;
    th.assert(v[0], 27);

    th.message("Delete index 0");
    v.del(0);
    th.assert();
    th.assert(v.size(), (std::size_t)1, "Size is 1");
    th.assert(v.capacity(), (std::size_t)2, "Capacity is 2");
  }

  std::srand((unsigned int)std::time(0));
  {
    Vector<int> v;
    std::vector<int> stdv;

    th.message("Stress test push");
    for(int i = 0; i < 5000; ++i) {
      if(v.to_std_vector() != stdv) {
        th.assert(false);
      }
      int r = std::rand();
      v.push(r);
      stdv.push_back(r);
    }
    th.assert();

    th.message("Stress test pop");
    for(int i = 0; i < 1000; ++i) {
      if(v.to_std_vector() != stdv) {
        th.assert(false);
      }
      int a = v.pop();
      int b = stdv.back();
      stdv.pop_back();
      if(a != b) {
        th.assert(a, b);
      }
    }
    th.assert();

    th.message("Stress test delete by index");
    for(int i = 0; i < 1000; ++i) {
      if(v.to_std_vector() != stdv) {
        th.assert(false);
      }
      unsigned int idx = std::rand() % v.size();
      v.del(idx);
      stdv.erase(stdv.begin() + idx);
    }
    th.assert();
  }

  th.summary();
  return 0;
}
