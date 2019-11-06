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
    void rm_edge();
    void restore();
    void evaporation();
    bool empty() const;
    size_t size() const;
    edge &longest_edge();

private:
    std::vector<edge> m_outgoing_edges;
    int16_t m_iterator = -1;
};

#endif // VERTEX_HPP
