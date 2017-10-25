#ifndef __STRUCTURES_GRAPH__
#define __STRUCTURES_GRAPH__

#include <cassert>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <unordered_map>

// TODO: Improve iterators meta-compatibility
class DiGraph {
public:
  typedef int vertex_type;
  typedef std::pair<vertex_type, vertex_type> edge_type;

public:
  virtual ~DiGraph() = default;

  virtual std::size_t vertex_count() const = 0;
  virtual void add_edge(edge_type e) = 0;
  virtual void remove_edge(edge_type e) = 0;
  virtual std::size_t edge_count() const = 0;
  virtual bool empty() const = 0;

//   virtual std::size_t degree(vertex_type n) const = 0;
//   virtual std::size_t in_degree(vertex_type n) const = 0;
//   virtual std::size_t out_degree(vertex_type n) const = 0;
//
// public:
//   class VertexIterator {
//   public:
//     virtual VertexIterator& operator++() = 0;
//     // virtual VertexIterator operator++(int) = 0;
//     // virtual bool operator==(const VertexIterator& other) const = 0;
//     // virtual bool operator!=(const VertexIterator& other) const = 0;
//     virtual vertex_type operator*() const = 0;
//     virtual bool end() const = 0;
//   };
//
//   class EdgeIterator {
//   public:
//     virtual EdgeIterator& operator++() = 0;
//     // virtual EdgeIterator operator++(int) = 0;
//     // virtual bool operator==(const EdgeIterator& other) const = 0;
//     // virtual bool operator!=(const EdgeIterator& other) const = 0;
//     virtual edge_type operator*() const = 0;
//     virtual bool end() const = 0;
//   };
//
// public:
//   virtual VertexIterator vertices() const = 0;
//   // virtual EdgeIterator edges() const = 0;
//   virtual VertexIterator adjacent(vertex_type n) const = 0;
};

// Adjacency List Directed Graph
class AdjacencyListDiGraph : public DiGraph {
public:
  ~AdjacencyListDiGraph() {
    clear();
  }

  void clear() {
    _vertices.clear();
  }

  void add_vertex(vertex_type v) {
    _vertices[v];
  }
  void remove_vertex(vertex_type v) {
    _vertices.erase(v);
    for (auto it = _vertices.begin(); it != _vertices.end(); ++it) {
      it->second.erase(v);
    }
  }
  std::size_t vertex_count() const {
    return _vertices.size();
  }

  void add_edge(edge_type e) {
    _vertices[e.first].insert(e.second);
  }
  void remove_edge(edge_type e) {
    _vertices[e.first].erase(e.second);
  }
  std::size_t edge_count() const {
    // TODO: improve speed by saving edge count
    return std::accumulate(
      _vertices.begin(), _vertices.end(), 0,
      [](std::size_t c, const list_type::value_type& a) {
        return c + a.second.size();
      }
    );
  }

  bool empty() const {
    return _vertices.empty();
  }

private:
  typedef std::unordered_set<vertex_type> adjacency_type;
  typedef std::unordered_map<vertex_type, adjacency_type> list_type;
  list_type _vertices;

public:
  class ALDEdgeIterator {
  public:
    ALDEdgeIterator(
      list_type::const_iterator lcurrent,
      list_type::const_iterator lend
    ) : _lcurrent(lcurrent), _lend(lend) {
      if (!end()) {
        _acurrent = lcurrent->second.begin();
        skip_empty();
      }
    }

    ALDEdgeIterator& operator++() {
      ++_acurrent;
      skip_empty();
      return *this;
    }

    edge_type operator*() const {
      return {_lcurrent->first, *_acurrent};
    }

    bool end() const {
      return _lcurrent == _lend;
    }
  private:
    list_type::const_iterator _lcurrent;
    adjacency_type::const_iterator _acurrent;
    list_type::const_iterator _lend;

    void skip_empty() {
      // skip empty adjacency lists
      while (!end() && _acurrent == _lcurrent->second.end()) {
        ++_lcurrent;
        if(!end()) {
          _acurrent = _lcurrent->second.begin();
        }
      }
    }
  };

public:
  ALDEdgeIterator edges() const {
    return ALDEdgeIterator(
      _vertices.begin(),
      _vertices.end()
    );
  }

  ALDEdgeIterator adjacent(vertex_type n) const {
    auto it = _vertices.find(n);
    return ALDEdgeIterator(
      it, it == _vertices.end() ? it : std::next(it)
    );
  }
};

// Adjacency Matrix Directed Graph
class AdjacencyMatrixDiGraph : public DiGraph {
public:
  AdjacencyMatrixDiGraph(const std::size_t nvertices = 0) :
  _nvertices(nvertices), _vertices(nvertices * nvertices) {
  }

  std::size_t vertex_count() const {
    return _nvertices;
  }

  void add_edge(edge_type e) {
    assert(e.first < _nvertices);
    assert(e.second < _nvertices);
    _vertices[_idx(e.first, e.second)] = true;
  }
  void remove_edge(edge_type e) {
    assert(e.first < _nvertices);
    assert(e.second < _nvertices);
    _vertices[_idx(e.first, e.second)] = false;
  }
  std::size_t edge_count() const {
    // TODO: improve speed by saving edge count
    return std::count(_vertices.begin(), _vertices.end(), true);
  }

  bool empty() const {
    return _nvertices == 0;
  }

private:
  typedef std::vector<bool> list_type;
  std::size_t _nvertices;
  list_type _vertices;

  std::size_t _idx(vertex_type a, vertex_type b) const {
    return a * _nvertices + b;
  }

public:
  class AMDEdgeIterator {
  public:
    AMDEdgeIterator(
      const list_type& v,
      std::size_t current,
      std::size_t end
    ) : _nv(sqrt(v.size())), _v(v), _current(current), _end(end) {
      skip_empty();
    }

    AMDEdgeIterator& operator++() {
      ++_current;
      skip_empty();
      return *this;
    }

    edge_type operator*() const {
      return { (vertex_type)(_current / _nv), (vertex_type)(_current % _nv) };
    }

    bool end() const {
      return _current >= _end;
    }

  private:
    std::size_t _nv;
    const list_type& _v;
    std::size_t _current;
    std::size_t _end;

    void skip_empty() {
      while (!end() && !_v[_current]) {
        ++_current;
      }
    }
  };

public:
  AMDEdgeIterator edges() const {
    return AMDEdgeIterator(_vertices, 0, _idx(_nvertices, 0));
  }

  AMDEdgeIterator adjacent(vertex_type n) const {
    assert(n < vertex_count());
    return AMDEdgeIterator(_vertices, _idx(n, 0), _idx(n + 1, 0));
  }
};

#endif
