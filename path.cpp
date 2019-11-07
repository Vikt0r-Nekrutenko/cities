#include "path.hpp"
#include <iostream>
path::path(size_t size) : m_passed_edges() { }

void path::print() const
{
    for(size_t i = 0; i < m_passed_edges.size(); i++)
        std::cout << m_passed_edges[i]->get_word() << "...";
}

void path::clear()
{
    m_length = 0u;
    m_passed_edges.clear();
}

void path::add_passed_edge(edge &ed)
{
    m_passed_edges.push_back(&ed);
    m_length += m_passed_edges.back()->get_word().length();
}

void path::rm_last_edge()
{
    m_length -= m_passed_edges.back()->get_word().length();
    m_passed_edges.pop_back();
}

std::vector<edge *>::iterator path::begin()
{
    return m_passed_edges.begin();
}

std::vector<edge *>::iterator path::end()
{
    return m_passed_edges.end();
}

const size_t &path::length() const
{
    return m_length;
}

const edge *path::last_edge() const
{
    return m_passed_edges.back();
}
