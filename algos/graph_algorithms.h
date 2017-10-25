#ifndef __ALGORITHMS_GRAPH__
#define __ALGORITHMS_GRAPH__

#include <cassert>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>

// based on http://www.boost.org/doc/libs/1_65_1/libs/graph/doc/BFSVisitor.html
template<class G>
struct BFSVisitor {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  // This is invoked when a vertex is encountered for the first time.
  virtual void discover_vertex(vertex_type v, const G& g) { }

  // This is invoked on a vertex as it is popped from the queue.
  // This happens immediately before examine_edge() is invoked on each of
  // the out-edges of vertex u.
  virtual void examine_vertex(vertex_type v, const G& g) { }

  // This is invoked on every out-edge of each vertex after it is discovered.
  virtual void examine_edge(edge_type e, const G& g) { }

  // This is invoked on each edge as it becomes a member of the edges that
  // form the search tree.
  virtual void tree_edge(edge_type e, const G& g) { }

  // This is invoked on back or cross edges for directed graphs and
  // cross edges for undirected graphs.
  virtual void non_tree_edge(edge_type e, const G& g) { }

  // This is invoked on the subset of non-tree edges whose target
  // vertex is colored gray at the time of examination. The color gray
  // indicates that the vertex is currently in the queue.
  virtual void frontier_target(edge_type e, const G& g) { }

  // This is invoked on the subset of non-tree edges whose target vertex
  // is colored black at the time of examination. The color black indicates
  // that the vertex has been removed from the queue.
  virtual void examined_target(edge_type e, const G& g) { }

  // This invoked on a vertex after all of its out edges have been added
  // to the search tree and all of the adjacent vertices have been
  // discovered (but before the out-edges of the adjacent vertices
  // have been examined).
  virtual void finish_vertex(vertex_type v, const G& g) { }
};

template<class G, class _Visitor, typename vertex_type>
void bfs(const G& g, vertex_type v, _Visitor visitor) {
  std::queue<vertex_type> q;
  enum color { GRAY, BLACK }; // implicit WHITE
  std::unordered_map<vertex_type, color> colors;

  visitor.discover_vertex(v, g);
  q.push(v);
  colors[v] = GRAY;

  while (!q.empty()) {
    auto top = q.front();
    q.pop();
    visitor.examine_vertex(top, g);

    for (auto edge_it = g.adjacent(top); !edge_it.end(); ++edge_it) {
      auto edge = *edge_it;
      auto target = edge.second;
      visitor.examine_edge(edge, g);

      auto color_it = colors.find(target);
      if (color_it == colors.end()) {
        // WHITE
        visitor.tree_edge(edge, g);
        q.push(target);
        colors[target] = GRAY;
        visitor.discover_vertex(target, g);
      } else if (colors[target] == GRAY) {
        // GRAY
        visitor.non_tree_edge(edge, g);
        visitor.frontier_target(edge, g);
      } else {
        // BLACK
        visitor.non_tree_edge(edge, g);
        visitor.examined_target(edge, g);
      }
    }

    colors[top] = BLACK;
    visitor.finish_vertex(top, g);
  }
}

// based on http://www.boost.org/doc/libs/1_65_1/libs/graph/doc/DFSVisitor.html
template<class G>
struct DFSVisitor {
  typedef typename G::vertex_type vertex_type;
  typedef typename G::edge_type edge_type;

  // This is invoked on the source vertex once before the start of the search.
  virtual void start_vertex(vertex_type v, const G& g) { }

  // This is invoked when a vertex is encountered for the first time.
  virtual void discover_vertex(vertex_type v, const G& g) { }

  // This is invoked on every out-edge of each vertex after it is discovered.
  virtual void examine_edge(edge_type e, const G& g) { }

  // This is invoked on each edge as it becomes a member of the edges that
  // form the search tree.
  virtual void tree_edge(edge_type e, const G& g) { }

  // This is invoked on the back edges in the graph.
  // For an undirected graph there is some ambiguity between tree edges
  // and back edges since the edge (u,v) and (v,u) are the same edge,
  // but both the tree_edge() and back_edge() functions will be invoked.
  // One way to resolve this ambiguity is to record the tree edges,
  // and then disregard the back-edges that are already marked as tree edges.
  // An easy way to record tree edges is to record predecessors
  // at the tree_edge event point.
  virtual void back_edge(edge_type e, const G& g) { }

  // This is invoked on forward or cross edges in the graph.
  // In an undirected graph this method is never called.
  virtual void forward_or_cross_edge(edge_type e, const G& g) { }

  // This is invoked on vertex u after finish_vertex has been called for
  // all the vertices in the DFS-tree rooted at vertex u.
  // If vertex u is a leaf in the DFS-tree, then the finish_vertex function
  // is called on u after all the out-edges of u have been examined.
  virtual void finish_vertex(vertex_type v, const G& g) { }
};

template<class G, class _Visitor, typename vertex_type>
void dfs(const G& g, vertex_type v, _Visitor visitor) {
  enum color { GRAY, BLACK }; // implicit WHITE
  enum paren { START, END };
  std::unordered_map<vertex_type, color> colors;
  std::stack<std::pair<vertex_type, paren>> s;

  visitor.discover_vertex(v, g);
  s.push({ v, START });
  colors[v] = GRAY;

  while(!s.empty()) {
    auto top_pair = s.top();
    s.pop();

    auto top = top_pair.first;
    auto state = top_pair.second;

    if (state == START) {
      visitor.start_vertex(top, g);
      s.push({ top, END });

      for (auto edge_it = g.adjacent(top); !edge_it.end(); ++edge_it) {
        auto edge = *edge_it;
        auto target = edge.second;
        visitor.examine_edge(edge, g);

        auto color_it = colors.find(target);
        if (color_it == colors.end()) {
          // WHITE
          visitor.tree_edge(edge, g);
          visitor.discover_vertex(target, g);
          s.push({ target, START });
          colors[target] = GRAY;
        } else if (colors[target] == GRAY) {
          // GRAY
          visitor.back_edge(edge, g);
        } else {
          // BLACK
          visitor.forward_or_cross_edge(edge, g);
        }
      }
    } else {
      colors[top] = BLACK;
      visitor.finish_vertex(top, g);
    }
  }
}

#endif
