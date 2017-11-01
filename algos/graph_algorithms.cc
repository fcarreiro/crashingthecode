#include <iostream>
#include <cstdlib>
#include "../structures/graph.h"
#include "graph_algorithms.h"
#include "../test_helpers.h"

// BFS visitors
template<typename G>
struct BFSPrinter : public BFSVisitor<G> {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  void examine_vertex(vertex_type v, const G& g) {
    std::cout << v << " ";
  }
};

template<typename G>
struct BFSTreePrinter : public BFSVisitor<G> {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  void tree_edge(edge_type e, const G& g) {
    std::cout << "(" << e.first << "," << e.second << ") ";
  }
};

template<typename G>
struct BFSExaminedEdgePrinter : public BFSVisitor<G> {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  void examined_target(edge_type e, const G& g) {
    std::cout << "(" << e.first << "," << e.second << ") ";
  }
};

template<typename G>
struct BFSFrontierEdgePrinter : public BFSVisitor<G> {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  void frontier_target(edge_type e, const G& g) {
    std::cout << "(" << e.first << "," << e.second << ") ";
  }
};

template<typename G>
struct BFSDistancePrinter : public BFSVisitor<G> {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  void tree_edge(edge_type e, const G& g) {
    auto source = e.first;
    auto target = e.second;
    // if distances[source] does not exist it is automatically
    // filled with std::size_t() which is 0
    distances[target] = distances[source] + 1;
  }

  void examine_vertex(vertex_type v, const G& g) {
    std::cout << v << "(" << distances[v] << ") ";
  }

  // TODO: visitors are passed by value so it's not nice to have this here
  std::unordered_map<vertex_type, std::size_t> distances;
};

// DFS visitors
template<typename G>
struct DFSPrinter : public DFSVisitor<G> {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  void start_vertex(vertex_type v, const G& g) {
    std::cout << v << " ";
  }
};

template<typename G>
struct DFSBackEdgePrinter : public DFSVisitor<G> {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  void back_edge(edge_type e, const G& g) {
    std::cout << "(" << e.first << "," << e.second << ") ";
  }
};

// Main entry point
int main(int argc, char const *argv[]) {
  TestHelper th;

  {
    AdjacencyMatrixDiGraph g(10);
    g.add_edge({ 1, 2 });
    g.add_edge({ 2, 3 });
    g.add_edge({ 2, 8 });
    g.add_edge({ 2, 9 });
    g.add_edge({ 3, 8 });
    g.add_edge({ 8, 5 });
    g.add_edge({ 5, 6 });
    g.add_edge({ 5, 9 });
    g.add_edge({ 6, 1 });

    std::cout << "BFS(1): ";
    bfs(g, 1, BFSPrinter<AdjacencyMatrixDiGraph>());
    std::cout << std::endl;

    std::cout << "BFS-Tree(1): ";
    bfs(g, 1, BFSTreePrinter<AdjacencyMatrixDiGraph>());
    std::cout << std::endl;

    std::cout << "BFS-FrontierEdge(1): ";
    bfs(g, 1, BFSFrontierEdgePrinter<AdjacencyMatrixDiGraph>());
    std::cout << std::endl;

    std::cout << "BFS-ExaminedEdge(1): ";
    bfs(g, 1, BFSExaminedEdgePrinter<AdjacencyMatrixDiGraph>());
    std::cout << std::endl;

    std::cout << "BFS-Distance(1): ";
    bfs(g, 1, BFSDistancePrinter<AdjacencyMatrixDiGraph>());
    std::cout << std::endl;

    std::cout << "DFS(1): ";
    dfs(g, 1, DFSPrinter<AdjacencyMatrixDiGraph>());
    std::cout << std::endl;

    std::cout << "DFS-BackEdges(1): ";
    dfs(g, 1, DFSBackEdgePrinter<AdjacencyMatrixDiGraph>());
    std::cout << std::endl;
  }

  {
    WeightedAdjacencyListDiGraph<int> wg;
    wg.set_edge_weight({ 1, 2 }, 1);
    wg.set_edge_weight({ 2, 3 }, 2);
    wg.set_edge_weight({ 2, 8 }, 3);
    wg.set_edge_weight({ 2, 9 }, 4);
    // wg.set_edge_weight({ 3, 6 }, 1);
    wg.set_edge_weight({ 8, 5 }, 5);
    wg.set_edge_weight({ 5, 6 }, 5);
    wg.set_edge_weight({ 6, 1 }, 7);
    wg.set_edge_weight({ 10, 20 }, 1);

    for (auto it = wg.adjacent(2); !it.end(); ++it) {
      std::cout << (*it).first.second << "(" << (*it).second << ") ";
    }
    std::cout << std::endl;

    auto distances = dijkstra(wg, 2);
    std::cout << "Dijkstra(2,1): "
              << (distances.count(1) == 1 ? distances[1] : -1);
    std::cout << std::endl;

    distances = bellman_ford(wg, 2);
    std::cout << "Bellman-Ford(2,1): "
              << (distances.count(1) == 1 ? distances[1] : -1);
    std::cout << std::endl;
  }

  {
    WeightedAdjacencyListDiGraph<int> wg;
    wg.set_edge_weight({ 1, 2 }, 1);
    wg.set_edge_weight({ 2, 3 }, -1);
    wg.set_edge_weight({ 2, 8 }, -11);
    wg.set_edge_weight({ 2, 9 }, 4);
    wg.set_edge_weight({ 3, 6 }, 1);
    wg.set_edge_weight({ 8, 5 }, 5);
    wg.set_edge_weight({ 5, 6 }, 5);
    wg.set_edge_weight({ 6, 1 }, 7);
    wg.set_edge_weight({ 10, 20 }, 1);

    auto distances = bellman_ford(wg, 2);
    std::cout << "Bellman-Ford(2,1): "
              << (distances.count(1) == 1 ? distances[1] : -1);
    std::cout << std::endl;
  }

  {
    AdjacencyListDiGraph<std::string> g;
    g.add_edge({ "undershorts", "shoes" });
    g.add_edge({ "undershorts", "pants" });
    g.add_edge({ "socks", "shoes" });
    g.add_edge({ "pants", "belt" });
    g.add_edge({ "shirt", "belt" });
    g.add_edge({ "shirt", "tie" });
    g.add_edge({ "tie", "jacket" });
    g.add_edge({ "belt", "jacket" });
    g.add_vertex("watch");

    std::cout << "All vertices: ";
    for (const auto& v : g.vertices()) {
      std::cout << v << " ";
    }
    std::cout << std::endl;

    std::cout << "Connected components: ";
    for (const auto& cc : connected_components<AdjacencyListDiGraph<std::string>,std::string>(g)) {
      std::cout << "[ ";
      for (const auto& elem : cc) {
        std::cout << elem << ", ";
      }
      std::cout << "] ";
    }
    std::cout << std::endl;
  }

  th.summary();
  return 0;
}
