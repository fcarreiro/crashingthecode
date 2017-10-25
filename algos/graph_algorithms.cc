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

  th.summary();
  return 0;
}
