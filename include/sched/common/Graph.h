#ifndef SCHED_COMMON_GRAPH_H
#define SCHED_COMMON_GRAPH_H

#include <set>
#include <vector>

#include "Range.h"
#include "RawId.h"

namespace sched {

  /*
   * VertexId
   */

  enum class VertexId : std::size_t { };
  using VertexRange = Range<VertexId>;

  constexpr VertexId NoVertex = VertexId{RawNoId};

  constexpr
  std::size_t to_index(VertexId id) {
    return static_cast<std::size_t>(id);
  }

  /*
   * EdgeId
   */

  enum class EdgeId : std::size_t { };
  using EdgeRange = Range<EdgeId>;

  constexpr EdgeId NoEdge = EdgeId{RawNoId};

  constexpr
  std::size_t to_index(EdgeId id) {
    return static_cast<std::size_t>(id);
  }

  /*
   * Graph
   */

  class Graph {
  public:
    struct Vertex {
      VertexId id;
    };

    struct Edge {
      EdgeId id;
      VertexId source;
      VertexId target;
    };

    explicit Graph(std::size_t n = 0);

    // vertices

    VertexId add_vertex();
    VertexRange vertices() const;
    std::size_t vertex_count() const;
    bool is_valid(VertexId v) const;
    void remove_vertex(VertexId v);

    // edges

    EdgeId add_edge(VertexId source, VertexId target);
    EdgeRange edges() const;
    std::size_t edge_count() const;
    bool is_valid(EdgeId e) const;
    void remove_edge(EdgeId e);

    VertexId source(EdgeId e) const;
    VertexId target(EdgeId e) const;

    // in edges

    using InEdgeRange = IteratorRange<std::set<EdgeId>::const_iterator>;

    InEdgeRange in_edges(VertexId v) const;

    // out edges

    using OutEdgeRange = IteratorRange<std::set<EdgeId>::const_iterator>;

    OutEdgeRange out_edges(VertexId v) const;

    //

    void clear();

  private:
    void erase_edge(Edge& edge);

    std::size_t m_next_vertex_id = 0;
    std::size_t m_next_edge_id = 0;
    std::size_t m_vertex_count = 0;
    std::size_t m_edge_count = 0;
    std::vector<Vertex> m_vertices;
    std::vector<Edge> m_edges;
    std::vector<std::set<EdgeId>> m_in_edges;
    std::vector<std::set<EdgeId>> m_out_edges;
  };


  template<typename V, typename E>
  class DataGraph : public Graph {
  public:
    explicit DataGraph(std::size_t n)
    : Graph(n)
    {
      if (n > 0) {
        m_vertex_data.reserve(n);
        m_edge_data.reserve(n);
      }
    }

    VertexId add_vertex(V v) {
      auto id = Graph::add_vertex();
      m_vertex_data.push_back(std::move(v));
      return id;
    }

    V& operator()(VertexId id) {
      return m_vertex_data[to_index(id)];
    }

    const V& operator()(VertexId id) const {
      return m_vertex_data[to_index(id)];
    }

    EdgeId add_edge(VertexId source, VertexId target, E e) {
      auto id = Graph::add_edge(source, target);
      m_edge_data.push_back(std::move(e));
      return id;
    }

    E& operator()(EdgeId id) {
      return m_edge_data[to_index(id)];
    }

    const E& operator()(EdgeId id) const {
      return m_edge_data[to_index(id)];
    }

  private:
    std::vector<V> m_vertex_data;
    std::vector<E> m_edge_data;
  };

}

#endif // SCHED_COMMON_GRAPH_H
