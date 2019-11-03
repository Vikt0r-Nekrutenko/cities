#include "path.hpp"

path::path() : m_pl(0.f)
{

}

float path::get_pl() const
{
    return m_pl;
}

void path::add_passed_edge(edge_t &edge)
{
    push_back(edge);
    m_pl += edge.word.length();
}

void path::rm_passed_edge()
{
    m_pl -= back().word.length();
    pop_back();
}
