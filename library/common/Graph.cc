#include <sched/common/Graph.h>

namespace sched {

  Graph::Graph(std::size_t n)
  : m_next_vertex_id(0)
  , m_next_edge_id(0)
  {
    if (n > 0) {
      m_vertices.reserve(n);
      m_out_edges.reserve(n);
    }
  }

  VertexId Graph::add_vertex() {
    auto id = VertexId{ m_next_vertex_id++ };
    m_vertices.push_back({ id });
    m_in_edges.push_back({ });
    m_out_edges.push_back({ });
    return id;
  }

  VertexRange Graph::vertices() const {
    return VertexRange{ m_vertices.size() };
  }

  std::size_t Graph::vertex_count() const {
    return m_vertices.size();
  }

  EdgeId Graph::add_edge(VertexId source, VertexId target) {
    auto id = EdgeId{ m_next_edge_id++ };
    m_edges.push_back({ id, source, target });
    m_in_edges[to_index(target)].insert(id);
    m_out_edges[to_index(source)].insert(id);
    return id;
  }

  EdgeRange Graph::edges() const {
    return EdgeRange{ m_edges.size() };
  }

  std::size_t Graph::edge_count() const {
    return m_edges.size();
  }

  VertexId Graph::source(EdgeId e) const {
    return m_edges[to_index(e)].source;
  }

  VertexId Graph::target(EdgeId e) const {
    return m_edges[to_index(e)].target;
  }

  auto Graph::in_edges(VertexId v) const -> InEdgeRange {
    return make_iterator_range(m_in_edges[to_index(v)]);
  }

  auto Graph::out_edges(VertexId v) const -> OutEdgeRange {
    return make_iterator_range(m_out_edges[to_index(v)]);
  }

  void Graph::clear() {
    m_next_vertex_id = 0;
    m_next_edge_id = 0;
    m_vertices.clear();
    m_edges.clear();
    m_in_edges.clear();
    m_out_edges.clear();
  }

}
