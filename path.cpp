#include "path.hpp"
#include <iostream>
path::path(size_t size) : m_passed_edges(size) { }

void path::print()
{
    for(size_t i = 0; i < m_iterator; i++)
        std::cout << m_passed_edges[i]->get_word() << "...";
}

void path::clear()
{
    m_length = 0u;
    m_iterator = 0u;
}

void path::add_passed_edge(const edge &ed)
{
    m_length += ed.get_word().length();
    m_passed_edges[m_iterator++] = &ed;
}

const size_t &path::length() const
{
    return m_length;
}
