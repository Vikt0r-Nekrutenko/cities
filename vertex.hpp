#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "edge.hpp"
#include <vector>

class vertex
{
public:
    void print();
    void sort();

    void add_edge(edge ed);
    bool empty() const;
    size_t size() const;

private:
    std::vector<edge> m_outgoing_edges;
};

#endif // VERTEX_HPP
