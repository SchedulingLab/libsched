// clang-format off: main header
#include <sched/common/Graph.h>
// clang-format on

#include <cassert>

namespace sched {

  Graph::Graph(std::size_t n)
  {
    if (n > 0) {
      m_vertices.reserve(n);
      m_out_edges.reserve(n);
    }
  }

  VertexId Graph::add_vertex()
  {
    auto id = VertexId{ m_next_vertex_id++ };
    m_vertices.push_back({ id });
    m_in_edges.emplace_back();
    m_out_edges.emplace_back();
    ++m_vertex_count;
    return id;
  }

  VertexRange Graph::vertices() const
  {
    return VertexRange{ m_vertices.size() };
  }

  std::size_t Graph::vertex_count() const
  {
    return m_vertex_count;
  }

  bool Graph::is_valid(VertexId v) const
  {
    return m_vertices[to_index(v)].id != NoVertex;
  }

  void Graph::remove_vertex(VertexId v)
  {
    assert(is_valid(v));
    m_vertices[to_index(v)].id = NoVertex;

    for (auto id : m_in_edges[to_index(v)]) {
      erase_edge(m_edges[to_index(id)]);
    }

    m_in_edges[to_index(v)].clear();

    for (auto id : m_out_edges[to_index(v)]) {
      erase_edge(m_edges[to_index(id)]);
    }

    m_out_edges[to_index(v)].clear();
  }

  EdgeId Graph::add_edge(VertexId source, VertexId target)
  {
    auto id = EdgeId{ m_next_edge_id++ };
    m_edges.push_back({ id, source, target });
    m_in_edges[to_index(target)].insert(id);
    m_out_edges[to_index(source)].insert(id);
    ++m_edge_count;
    return id;
  }

  EdgeRange Graph::edges() const
  {
    return EdgeRange{ m_edges.size() };
  }

  std::size_t Graph::edge_count() const
  {
    return m_edge_count;
  }

  bool Graph::is_valid(EdgeId e) const
  {
    return m_edges[to_index(e)].id != NoEdge;
  }

  void Graph::remove_edge(EdgeId e)
  {
    assert(is_valid(e));
    [[maybe_unused]] std::size_t erased = 0;
    Edge& edge = m_edges[to_index(e)];
    erased = m_in_edges[to_index(edge.target)].erase(e);
    assert(erased == 1);
    erased = m_out_edges[to_index(edge.source)].erase(e);
    assert(erased == 1);
    erase_edge(edge);
  }

  VertexId Graph::source(EdgeId e) const
  {
    return m_edges[to_index(e)].source;
  }

  VertexId Graph::target(EdgeId e) const
  {
    return m_edges[to_index(e)].target;
  }

  auto Graph::in_edges(VertexId v) const -> InEdgeRange
  {
    return make_iterator_range(m_in_edges[to_index(v)]);
  }

  auto Graph::out_edges(VertexId v) const -> OutEdgeRange
  {
    return make_iterator_range(m_out_edges[to_index(v)]);
  }

  void Graph::clear()
  {
    m_next_vertex_id = 0;
    m_next_edge_id = 0;
    m_vertices.clear();
    m_edges.clear();
    m_in_edges.clear();
    m_out_edges.clear();
  }

  void Graph::erase_edge(Edge& edge)
  {
    if (edge.id == NoEdge) {
      return;
    }

    edge.id = NoEdge;
    edge.source = NoVertex;
    edge.target = NoVertex;
    --m_edge_count;
  }

}
