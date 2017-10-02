#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../test_helpers.h"

template<class T, class ArrayLike>
std::size_t binary_search(const T& value, const ArrayLike& v) {
  if(v.empty()) {
    return 0;
  }

  std::size_t min = 0;
  std::size_t max = v.size() - 1;
  std::size_t mid;

  while(min < max) {
    mid = (std::size_t)std::ceil((min + max + 1) / 2);
    if(v[mid] == value) {
      return mid;
    } else if(v[mid] < value) {
      min = mid + 1;
    } else if(v[mid] > value) {
      max = mid - 1;
    }
  }

  // min == max
  return (v[min] == value) ? min : v.size();
}

int main(int argc, char const *argv[]) {
  TestHelper th;
  std::srand((unsigned int)std::time(0));

  std::cout << "\n[[ Binary Search ]]" << std::endl << std::endl;

  th.message("Testing on random vectors");
  for(auto i = 0; i < 500; ++i) {
    std::vector<int> v(std::rand() % 1000);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());

    for(auto j = 0; j < 200; ++j) {
      auto n = std::rand();
      auto bsindex = binary_search(n, v);
      auto stdbs = std::binary_search(v.begin(), v.end(), n);
      th.tassert(stdbs, bsindex != v.size(), "Found?", true);
      if(bsindex != v.size()) {
        th.tassert(v[bsindex], n, "Correct index", true);
      }
    }
  }
  th.tassert();

  th.summary();
  return 0;
}
