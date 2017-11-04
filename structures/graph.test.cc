#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "graph.h"
#include "../test_helpers.h"

int main(int argc, char const *argv[]) {
  TestHelper th;

  {
    AdjacencyMatrixDiGraph g(10);
    g.add_edge({ 1, 2 });
    g.add_edge({ 2, 3 });
    g.add_edge({ 2, 8 });
    g.add_edge({ 2, 9 });
    g.add_edge({ 8, 5 });
    g.add_edge({ 5, 6 });
    g.add_edge({ 6, 1 });

    for (auto it = g.adjacent(2); !it.end(); ++it) {
      std::cout << (*it).second << " ";
    }
    std::cout << std::endl;

    std::cout << "All edges: ";
    for (auto it = g.edges(); !it.end(); ++it) {
      std::cout << "(" << (*it).first << "," << (*it).second << ") ";
    }
    std::cout << std::endl;
  }

  {
    AdjacencyListDiGraph<std::string> g;
    g.add_edge({ "a", "b" });
    g.add_edge({ "b", "c" });
    g.add_edge({ "b", "8" });
    g.add_edge({ "b", "9" });
    g.add_edge({ "8", "5" });
    g.add_edge({ "5", "6" });
    g.add_edge({ "6", "1" });

    for (auto it = g.adjacent("b"); !it.end(); ++it) {
      std::cout << (*it).second << " ";
    }
    std::cout << std::endl;

    std::cout << "All edges: ";
    for (auto it = g.edges(); !it.end(); ++it) {
      std::cout << "(" << (*it).first << "," << (*it).second << ") ";
    }
    std::cout << std::endl;
  }

  {
    WeightedAdjacencyListDiGraph<int> wg;
    wg.set_edge_weight({ 1, 2 }, 1);
    wg.set_edge_weight({ 2, 3 }, 2);
    wg.set_edge_weight({ 2, 8 }, 3);
    wg.set_edge_weight({ 2, 9 }, 4);
    wg.set_edge_weight({ 8, 5 }, 5);
    wg.set_edge_weight({ 5, 6 }, 5);
    wg.set_edge_weight({ 6, 1 }, 7);

    for (auto it = wg.adjacent(2); !it.end(); ++it) {
      std::cout << (*it).first.second << "(" << (*it).second << ") ";
    }
    std::cout << std::endl;

    std::cout << "All edges: ";
    for (auto it = wg.edges(); !it.end(); ++it) {
      std::cout << "(" << (*it).first.first << ","
                << (*it).first.second << "):"
                << (*it).second << " ";
    }
    std::cout << std::endl;
  }

  {
    AdjacencyMatrixDiGraph g(10);
    g.add_edge({ 1, 2 });
    g.add_edge({ 2, 3 });
    g.add_edge({ 2, 8 });
    g.add_edge({ 2, 9 });
    g.add_edge({ 8, 5 });
    g.add_edge({ 5, 6 });
    g.add_edge({ 6, 1 });

    for (auto it = g.adjacent(2); !it.end(); ++it) {
      std::cout << (*it).second << " ";
    }
    std::cout << std::endl;

    std::cout << "All edges: ";
    for (auto it = g.edges(); !it.end(); ++it) {
      std::cout << "(" << (*it).first << "," << (*it).second << ") ";
    }
    std::cout << std::endl;
  }

  {
    AdjacencyListGraph<std::string> g;
    g.add_edge({ "a", "b" });
    g.add_edge({ "b", "c" });
    g.add_edge({ "b", "8" });
    g.add_edge({ "b", "9" });

    std::cout << "(undirected graph) All edges: ";
    for (auto it = g.edges(); !it.end(); ++it) {
      std::cout << "(" << (*it).first << "," << (*it).second << ") ";
    }
    std::cout << std::endl;
  }

  {
    WeightedAdjacencyListGraph<int> wg;
    wg.set_edge_weight({ 1, 2 }, 1);
    wg.set_edge_weight({ 2, 3 }, 2);
    wg.set_edge_weight({ 2, 8 }, 3);
    wg.set_edge_weight({ 2, 9 }, 4);
    wg.set_edge_weight({ 8, 5 }, 5);
    wg.set_edge_weight({ 5, 6 }, 5);
    wg.set_edge_weight({ 6, 1 }, 7);

    std::cout << "(undirected) All edges: ";
    for (auto it = wg.edges(); !it.end(); ++it) {
      std::cout << "(" << (*it).first.first << ","
                << (*it).first.second << "):"
                << (*it).second << " ";
    }
    std::cout << std::endl;
  }

  th.summary();
  return 0;
}
