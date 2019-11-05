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
    void add_passed_edge(const edge &ed);

    const size_t &length() const;
private:
    std::vector<const edge *> m_passed_edges;
    size_t m_length = 0u;
    size_t m_iterator = 0u;
};

#endif // PATH_HPP
