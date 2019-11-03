#ifndef PATH_HPP
#define PATH_HPP

#include "vertex.hpp"

class path : public vector<edge_t>
{
public:
    path();
    float get_pl() const;
    void add_passed_edge(edge_t &edge);
    void rm_passed_edge();
private:
    float m_pl;
};

#endif // PATH_HPP
