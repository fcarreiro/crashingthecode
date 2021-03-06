#ifndef __ALGORITHMS_GRAPH__
#define __ALGORITHMS_GRAPH__

#include <cassert>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <vector>
#include <list>

template<class G>
G transpose(const G& g) {
  G t;

  for (const auto& v : g.vertices()) {
    t.add_vertex(v);
  }

  for (auto edge_it = g.edges(); !edge_it.end(); ++edge_it) {
    t.add_edge({ (*edge_it).second, (*edge_it).first });
  }

  return t;
}

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

template<class G, class _Visitor>
void bfs(const G& g, typename G::vertex_type v, _Visitor visitor) {
  typedef typename G::vertex_type vertex_type;
  enum direction { IN, OUT };
  std::queue<std::pair<vertex_type, direction>> q;
  enum color { GRAY, BLACK }; // implicit WHITE
  std::unordered_map<vertex_type, color> colors;

  visitor.discover_vertex(v, g);
  q.push({ v, IN });
  colors[v] = GRAY;

  while (!q.empty()) {
    auto topv = q.front().first;
    auto topdir = q.front().second;
    q.pop();

    if (topdir == IN) {
      visitor.examine_vertex(topv, g);
      q.push({ topv, OUT });

      for (auto edge_it = g.adjacent(topv); !edge_it.end(); ++edge_it) {
        auto edge = *edge_it;
        auto target = edge.second;
        visitor.examine_edge(edge, g);

        auto color_it = colors.find(target);
        if (color_it == colors.end()) {
          // WHITE
          visitor.tree_edge(edge, g);
          q.push({ target, IN });
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
    } else {
      colors[topv] = BLACK;
      visitor.finish_vertex(topv, g);
    }
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

template<class G, class _Visitor>
void dfs(const G& g, typename G::vertex_type v, _Visitor visitor) {
  typedef typename G::vertex_type vertex_type;
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

template<class G, class _Visitor>
void global_dfs(const G& g, _Visitor visitor) {
  typedef typename G::vertex_type vertex_type;
  enum color { GRAY, BLACK }; // implicit WHITE
  enum paren { START, END };
  std::unordered_map<vertex_type, color> colors;
  std::stack<std::pair<vertex_type, paren>> s;

  for (const auto& v : g.vertices()) {
    if (colors.find(v) != colors.end()) {
      continue;
    }

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
}

// Dijkstra
template<class WG>
std::unordered_map<typename WG::vertex_type, std::size_t> dijkstra(const WG& g, typename WG::vertex_type s) {
  typedef typename WG::vertex_type vertex_type;
  typedef std::pair<vertex_type, std::size_t> vertex_dist;
  auto cmp = [](vertex_dist a, vertex_dist b) {
    return a.second > b.second;
  };
  std::priority_queue<vertex_dist, std::vector<vertex_dist>, decltype(cmp)> q(cmp);
  std::unordered_map<vertex_type, std::size_t> distance;
  std::unordered_set<vertex_type> done;

  q.push({ s, 0 });
  distance[s] = 0;

  while (!q.empty()) {
    vertex_dist vd = q.top();
    q.pop();
    auto x = vd.first;
    auto dx = vd.second;

    // there could be duplicates because we don't have decrease_key; see (*)
    if (done.count(x) == 0) {
      assert(dx == distance[x]);
      // std::cout << "current: " << x << std::endl;
      // mark x as finished
      done.insert(x);

      // relax adjacents
      for (auto edge_it = g.adjacent(x); !edge_it.end(); ++edge_it) {
        auto edge = *edge_it;
        auto target = edge.first.second;
        auto weight = edge.second;
        if (distance.count(target) == 0 || dx + weight < distance[target]) {
          distance[target] = dx + weight;
          q.push({ target, distance[target] }); // (*)
          // std::cout << "distance[" << target << "] = " << distance[target] << std::endl;
        }
      }
    }
  }

  return distance;
}

// Bellman-Ford
// Returns an empty map if there is a negative cycle
template<class WG>
std::unordered_map<typename WG::vertex_type, std::size_t> bellman_ford(const WG& g, typename WG::vertex_type s) {
  typedef typename WG::vertex_type vertex_type;
  std::unordered_map<vertex_type, std::size_t> distance;
  distance[s] = 0;

  // calculate distances
  for (std::size_t i = 0; i < g.vertex_count() - 1; ++i) {
    for (auto edge_it = g.edges(); !edge_it.end(); ++edge_it) {
      auto edge = *edge_it;
      auto source = edge.first.first;
      auto target = edge.first.second;
      auto weight = edge.second;

      // relaaaaaax
      if (distance.count(source) != 0) {
        auto ds = distance[source];
        if (distance.count(target) == 0 || ds + weight < distance[target]) {
          distance[target] = ds + weight;
        }
      }
    }
  }

  // check for negative cycles (could be merged with above loop but it's
  // easier to read this way)
  for (auto edge_it = g.edges(); !edge_it.end(); ++edge_it) {
    auto edge = *edge_it;
    auto source = edge.first.first;
    auto target = edge.first.second;
    auto weight = edge.second;

    // relaaaaaax
    if (distance.count(source) != 0 && distance[source] + weight < distance[target]) {
      // oops... there's a cycle
      return {};
    }
  }

  return distance;
}

template<class G>
std::list<std::unordered_set<typename G::vertex_type>> directed_connected_components(const G& g) {
  typedef typename G::vertex_type vertex_type;
  std::list<std::unordered_set<vertex_type>> ret;

  // do a first-pass global DFS and calculate
  std::list<vertex_type> reverse_finish_time;

  {
    struct FinishTimeDFSVisitor : public DFSVisitor<G> {
      FinishTimeDFSVisitor(std::list<vertex_type>& rft) : _rft(rft) { }

      void finish_vertex(vertex_type v, const G& g) {
        _rft.push_front(v);
      }

      std::list<vertex_type>& _rft;
    };

    FinishTimeDFSVisitor visitor(reverse_finish_time);
    global_dfs(g, visitor);
  }

  {
    G t = transpose(g);
    std::unordered_set<vertex_type> done;
    std::unordered_set<vertex_type> current_cc;

    struct CCDFSVisitor : public DFSVisitor<G> {
      CCDFSVisitor(std::unordered_set<vertex_type>& d, std::unordered_set<vertex_type>& cur) :
      _done(d), _cur(cur) {
      }
      void start_vertex(vertex_type v, const G& g) {
        if (_done.find(v) == _done.end()) {
          _done.insert(v);
          _cur.insert(v);
        }
      }
      std::unordered_set<vertex_type>& _done;
      std::unordered_set<vertex_type>& _cur;
    };

    // we can't use global DFS as it is because we need to pick the
    // vertices in the order given by "reverse_finish_time"
    // TODO: this also makes the complexity far worse,
    // because the dfs() call could visit the whole graph each time
    // this is only a proof of concept but otherwise that should be fixed
    CCDFSVisitor visitor(done, current_cc);

    for (const auto& s : reverse_finish_time) {
      dfs(t, s, visitor);
      if (!current_cc.empty()) {
        ret.push_back(current_cc);
        current_cc.clear();
      }
    }
  }

  return ret;
}

template<class G>
bool has_cycle(const G& g) {
  struct CycleDFSVisitor : public DFSVisitor<G> {
    CycleDFSVisitor(bool& cp) : _cp(cp) { }

    void back_edge(typename DFSVisitor<G>::edge_type e, const G& g) {
      _cp = true;
    }

    bool& _cp;
  };

  bool cycle_present = false;
  CycleDFSVisitor visitor(cycle_present);
  global_dfs(g, visitor);
  return cycle_present;
}

template<class G>
std::list<typename G::vertex_type> topological_sort(const G& g) {
  typedef typename G::vertex_type vertex_type;
  assert(!has_cycle(g));

  struct TopoDFSVisitor : public DFSVisitor<G> {
    TopoDFSVisitor(std::list<vertex_type>& rret) : _ret(rret) { }

    void finish_vertex(vertex_type v, const G& g) {
      _ret.push_front(v);
    }

    std::list<vertex_type>& _ret;
  };

  std::list<vertex_type> ret;
  TopoDFSVisitor visitor(ret);
  global_dfs(g, visitor);
  return ret;
}

// Prim's minimum spanning tree
template<class WG>
std::unordered_map<typename WG::vertex_type, typename WG::vertex_type> prim_mst(const WG& g) {
  typedef typename WG::vertex_type vertex_type;
  typedef typename WG::weight_type weight_type;
  typedef std::pair<vertex_type, weight_type> vertex_dist;
  auto cmp = [](vertex_dist a, vertex_dist b) {
    return a.second > b.second;
  };
  std::priority_queue<vertex_dist, std::vector<vertex_dist>, decltype(cmp)> q(cmp);
  std::unordered_map<vertex_type, vertex_type> parent;
  std::unordered_map<vertex_type, weight_type> key;
  std::unordered_set<vertex_type> done;

  if (g.empty()) {
    return parent;
  }

  auto random_root = *g.vertices().begin();
  q.push({ random_root, 0 });
  // parent[random_root] = random_root;
  key[random_root] = 0;

  while (!q.empty()) {
    vertex_dist vd = q.top();
    q.pop();
    auto x = vd.first;

    // there could be duplicates because we don't have decrease_key; see (*)
    if (done.count(x) == 0) {
      done.insert(x);

      // add adjacent vertices to queue
      for (auto edge_it = g.adjacent(x); !edge_it.end(); ++edge_it) {
        auto edge = *edge_it;
        auto target = edge.first.second;
        auto weight = edge.second;

        // we don't modify key or parent of vertices that have already been chosen
        if (done.count(target) == 0 && (key.count(target) == 0 || weight < key[target])) {
          key[target] = weight;
          parent[target] = x;
          q.push({ target, weight }); // (*)
        }
      }
    }
  }

  return parent;
}

#endif
