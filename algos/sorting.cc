#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../test_helpers.h"

template<class T, class ArrayLike>
void sort_insertion(ArrayLike& v) {
  for(std::size_t k = 1; k < v.size(); ++k) {
    // invariant: v[0..k) is sorted
    for(std::size_t i = k; i > 0 && v[i - 1] > v[i]; --i) {
      // invariant:
      //   i > 0 && v[i - 1] > v[i]
      //   v[0..i) is sorted
      //   all v[i..k] >= v[i]
      std::swap(v[i - 1], v[i]);
    }
  }
}

template<class T, class ArrayLike>
void sort_bubble(ArrayLike& v) {
  for(std::size_t k = v.size(); k > 0; --k) {
    // invariant:
    //   v[k..] is sorted
    //   all v[0..k) <= all v[k..]
    for(std::size_t i = 0; i < k - 1; ++i) {
      // invariant:
      //   v[i] >= all v[0..i]
      if(v[i] > v[i + 1]) {
        std::swap(v[i + 1], v[i]);
      }
    }
  }
}

template<class T, class ArrayLike>
void sort_selection(ArrayLike& v) {
  for(std::size_t k = 0; k < v.size(); ++k) {
    // invariant:
    //   v[0..k) is sorted
    //   all v[0..k) <= all v[k..)
    std::size_t min_idx = k;
    for(std::size_t i = k; i < v.size(); ++i) {
      // invariant:
      //   v[min_idx] <= all v[k..i)
      if(v[i] < v[min_idx]) {
        min_idx = i;
      }
    }
    std::swap(v[k], v[min_idx]);
  }
}

void test_sort(TestHelper& th, std::function<void(std::vector<int>&)> my_sort) {
  th.message("Testing on random vectors");
  for(auto i = 0; i < 2000; ++i) {
    std::vector<int> v(std::rand() % 1000);
    std::generate(v.begin(), v.end(), std::rand);
    std::vector<int> vtest(v);
    std::sort(vtest.begin(), vtest.end());
    my_sort(v);
    th.tassert(v == vtest, true, "Vector equality", true);
  }
  th.tassert();
}

int main(int argc, char const *argv[]) {
  TestHelper th;
  std::srand((unsigned int)std::time(0));

  std::cout << "\n[[ Insertion Sort ]]" << std::endl << std::endl;
  test_sort(th, sort_insertion<int, std::vector<int>>);

  std::cout << "\n[[ Bubble Sort ]]" << std::endl << std::endl;
  test_sort(th, sort_bubble<int, std::vector<int>>);

  std::cout << "\n[[ Selection Sort ]]" << std::endl << std::endl;
  test_sort(th, sort_selection<int, std::vector<int>>);

  th.summary();
  return 0;
}
