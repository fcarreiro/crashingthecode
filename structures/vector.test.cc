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
    th.tassert();
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Initializer list construction");
    Vector<int> v = {1, 2, 3, 4};
    th.tassert();
    th.tassert(v.size(), (std::size_t)4, "Size is 4");
    th.tassert(v.capacity(), (std::size_t)4, "Capacity is 4");
    th.message("Destruction");
  }
  th.tassert();

  {
    Vector<int> v = {1, 2, 3, 4};
    th.message("Copy construction");
    Vector<int> v2(v);
    th.tassert();
    th.tassert(v.to_std_vector() == v2.to_std_vector(), true, "STD vectors equal");
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("Move semantics construction");
    Vector<int> v2(Vector<int>{1, 2, 3, 4});
    th.tassert();
    th.tassert(v2.to_std_vector() == std::vector<int>{1, 2, 3, 4}, true, "STD vectors equal");
    th.message("Destruction");
  }
  th.tassert();

  {
    Vector<int> v = {1, 2, 3, 4};
    th.message("operator= (copy)");
    Vector<int> v2;
    v2 = v;
    th.tassert();
    th.tassert(v.to_std_vector() == v2.to_std_vector(), true, "STD vectors equal");
    th.message("Destruction");
  }
  th.tassert();

  {
    th.message("operator= (move)");
    Vector<int> v2;
    v2 = Vector<int>{1, 2, 3, 4};
    th.tassert();
    th.tassert(v2.to_std_vector() == std::vector<int>{1, 2, 3, 4}, true, "STD vectors equal");
    th.message("Destruction");
  }
  th.tassert();

  {
    Vector<int> v = {1, 2, 3, 4};
    th.message("Resize (shrink) to 2");
    v.resize(2);
    th.tassert();
    th.tassert(v.size(), (std::size_t)2, "Size is 2");
    th.tassert(v.capacity(), (std::size_t)2, "Capacity is 2");
    th.tassert(v.at(0), 1, "v.at(0) == 1");
    th.tassert(v.at(1), 2, "v.at(1) == 2");
    th.message("Resize (grow) to 8");
    v.resize(8);
    th.tassert();
    th.tassert(v.size(), (std::size_t)8, "Size is 8");
    th.tassert(v.capacity(), (std::size_t)8, "Capacity is 8");
    th.tassert(v.at(0), 1, "v.at(0) == 1");
    th.tassert(v.at(1), 2, "v.at(1) == 2");
    th.message("set v[4] = 7");
    v[4] = 7;
    th.tassert(v.at(4), 7, "v.at(4) == 7");
    th.tassert();
  }

  {
    Vector<int> v;
    th.tassert(v.capacity(), (std::size_t)1, "Initial capacity is 1");

    v.push_back(7);
    th.tassert(v.capacity(), (std::size_t)1, "Capacity after one push is 1");
    th.tassert(v.size(), (std::size_t)1, "Size after one push is 1");

    v.push_back(8);
    th.tassert(v.capacity(), (std::size_t)2, "Capacity after two pushes is 2");
    th.tassert(v.size(), (std::size_t)2, "Size after two pushes is 2");

    v.push_back(9);
    th.tassert(v.capacity(), (std::size_t)4, "Capacity after three pushes is 4");
    th.tassert(v.size(), (std::size_t)3, "Size after three pushes is 3");

    th.tassert(v.at(0), 7, "v.at(0) == 7");
    th.tassert(v[0], 7, "v[0] == 7");
    th.tassert(v.at(1), 8, "v.at(1) == 8");
    th.tassert(v[1], 8, "v[1] == 8");
    th.tassert(v.at(2), 9, "v.at(2) == 9");
    th.tassert(v[2], 9, "v[2] == 9");

    th.message("Insert 1 at offset 0");
    v.insert(0, 1);
    th.tassert();
    th.tassert(v[0], 1, "v[0] == 1");
    th.tassert(v[1], 7, "v[1] == 7");
    th.tassert(v[2], 8, "v[2] == 8");
    th.tassert(v[3], 9, "v[3] == 9");
    th.tassert(v.size(), (std::size_t)4, "Size is 4");

    th.message("Insert 5 at offset 2");
    v.insert(2, 5);
    th.tassert();
    th.tassert(v[0], 1, "v[0] == 1");
    th.tassert(v[1], 7, "v[1] == 7");
    th.tassert(v[2], 5, "v[2] == 5");
    th.tassert(v[3], 8, "v[3] == 8");
    th.tassert(v[4], 9, "v[4] == 9");
    th.tassert(v.size(), (std::size_t)5, "Size is 5");

    th.message("Pop");
    int last = v.pop_back();
    th.tassert();
    th.tassert(last, 9, "popped == 9");
    th.tassert(v[0], 1, "v[0] == 1");
    th.tassert(v[1], 7, "v[1] == 7");
    th.tassert(v[2], 5, "v[2] == 5");
    th.tassert(v[3], 8, "v[3] == 8");
    th.tassert(v.size(), (std::size_t)4, "Size is 4");
    th.tassert(v.capacity(), (std::size_t)8, "Capacity is 8");

    th.tassert(v.find(5), (std::size_t)2, "Element 5 is found at location 2");
    th.tassert(v.find(8), (std::size_t)3, "Element 8 is found at location 3");

    th.message("Remove 5");
    std::size_t five_idx = v.remove(5);
    th.tassert();
    th.tassert(five_idx, (std::size_t)2, "5 was at index 2");
    th.tassert(v[0], 1, "v[0] == 1");
    th.tassert(v[1], 7, "v[1] == 7");
    th.tassert(v[2], 8, "v[2] == 8");
    th.tassert(v.size(), (std::size_t)3, "Size is 3");
    th.tassert(v.capacity(), (std::size_t)8, "Capacity is 8");

    th.message("Delete index 0");
    int elem_at_zero = v.erase(0);
    th.tassert();
    th.tassert(elem_at_zero, 1, "Element at index 0 was 1");
    th.tassert(v[0], 7, "v[0] == 7");
    th.tassert(v[1], 8, "v[1] == 8");
    th.tassert(v.size(), (std::size_t)2, "Size is 2");
    th.tassert(v.capacity(), (std::size_t)8, "Capacity is 8");

    th.message("Change element at 0 to 27");
    v[0] = 27;
    th.tassert(v[0], 27);

    th.message("Delete index 0");
    v.erase(0);
    th.tassert();
    th.tassert(v.size(), (std::size_t)1, "Size is 1");
    th.tassert(v.capacity(), (std::size_t)4, "Capacity is 4");

    th.message("Pop last element");
    v.pop_back();
    th.tassert();
    th.tassert(v.size(), (std::size_t)0, "Size is 0");
    th.tassert(v.capacity(), (std::size_t)2, "Capacity is 2");

    th.message("Push after having emptied");
    v.push_back(55);
    th.tassert();
    th.tassert(v.capacity(), (std::size_t)2, "Capacity is 2");
    th.tassert(v.size(), (std::size_t)1, "Size is 1");
  }

  std::srand((unsigned int)std::time(0));
  {
    Vector<int> v;
    std::vector<int> stdv;

    th.message("Stress test push");
    for(int i = 0; i < 5000; ++i) {
      if(v.to_std_vector() != stdv) {
        th.tassert(false);
      }
      int r = std::rand();
      v.push_back(r);
      stdv.push_back(r);
      th.tassert(v.to_std_vector() == stdv, true, "Check complete vector", true);
    }
    th.tassert();

    th.message("Stress test pop");
    for(int i = 0; i < 1000; ++i) {
      if(v.to_std_vector() != stdv) {
        th.tassert(false);
      }
      int a = v.pop_back();
      int b = stdv.back();
      stdv.pop_back();
      if(a != b) {
        th.tassert(a, b, "Element returned by pop");
      }
      th.tassert(v.to_std_vector() == stdv, true, "Check complete vector", true);
    }
    th.tassert();

    th.message("Stress test delete by index");
    for(int i = 0; i < 1000; ++i) {
      if(v.to_std_vector() != stdv) {
        th.tassert(false);
      }
      unsigned int idx = std::rand() % v.size();
      v.erase(idx);
      stdv.erase(stdv.begin() + idx);
    }
    th.tassert();
  }

  th.summary();
  return 0;
}
