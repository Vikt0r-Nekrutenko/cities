#include "path.hpp"

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

std::vector<std::string> path::get_words() const
{
    std::vector<std::string> words;
    for(edge *edge : m_passed_edges)
        words.push_back(edge->get_word());
    return words;
}
