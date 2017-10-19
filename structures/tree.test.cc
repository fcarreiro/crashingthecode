#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "tree.h"
#include "../test_helpers.h"

int main(int argc, char const *argv[]) {
  TestHelper th;

  {
    th.message("Default construction");
    BinarySearchTree<int> t;
    th.tassert();
    th.message("Destruction");
  }
  th.tassert();

  {
    BinarySearchTree<int> t;
    t.insert(5);
    t.insert(2);
    t.insert(10);
    t.insert(1);
    t.insert(-5);
    t.insert(28);
    t.print();
    std::cout << "height: " << t.height() << std::endl;
    th.message("Destruction with elements");
  }
  th.tassert();

  {
    BinarySearchTree<int> t;
    std::vector<int> v;
    th.tassert(t.size(), (std::size_t)0, "Size is 0 before inserting");
    t.insert(5);
    t.insert(2);
    t.insert(10);
    t.insert(1);
    t.insert(-5);
    t.insert(28);
    th.tassert(t.size(), (std::size_t)6, "Size is 6 after inserting 6 items");
    t.visit_inorder([&v](int i) { v.push_back(i); });
    th.tassert(std::is_sorted(v.begin(), v.end()), "Inorder is sorted");
    th.tassert(t.find(10), "10 is there");
    th.tassert(t.find(8), false, "8 is not there");
    th.message("Remove 10");
    t.remove(10);
    th.tassert();
    th.tassert(t.size(), (std::size_t)5, "Size is 5");
    th.tassert(t.find(10), false, "10 is not there");
    th.message("Insert repeated element");
    t.insert(28);
    th.tassert();
    th.tassert(t.size(), (std::size_t)5, "Size is still 5");
  }

  th.summary();
  return 0;
}
