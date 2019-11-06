#ifndef PATH_HPP
#define PATH_HPP

#include "edge.hpp"
#include <vector>

class path
{
public:
    path(size_t size);
    void print();
    void clear();
    void add_passed_edge(edge &ed);
    void rm_last_edge();

    std::vector<edge *>::iterator begin();
    std::vector<edge *>::iterator end();

    const size_t &length() const;
    const edge *last_edge() const;
private:
    std::vector<edge *> m_passed_edges;
    size_t m_length = 0u;
};

#endif // PATH_HPP
