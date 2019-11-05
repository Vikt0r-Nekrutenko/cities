#include "vertex.hpp"
#include <iostream>

void vertex::print()
{
    for(auto i = m_outgoing_edges.begin(); i != m_outgoing_edges.begin() + m_iterator + 1; i++){
        std::cout << i->get_index() << " " << i->get_word() << " " << i->get_pheromone() << std::endl;
    }
}

void vertex::sort()
{
    for(std::vector<edge>::iterator it = m_outgoing_edges.begin(); it != m_outgoing_edges.end() - 1; it++){
        for(std::vector<edge>::iterator sub_it = it + 1; sub_it != m_outgoing_edges.end(); sub_it++)
            if(it->get_word().length() > sub_it->get_word().length()) {
                std::swap(*it, *sub_it);
            }
        it->set_index(++m_iterator);
    }
    m_outgoing_edges.back().set_index(++m_iterator);
}

void vertex::add_edge(edge ed)
{
    m_outgoing_edges.push_back(ed);
}

void vertex::rm_edge()
{
    m_iterator--;
}

void vertex::restore()
{
    m_iterator = m_outgoing_edges.size() - 1;
}

bool vertex::empty() const
{
    return m_iterator == -1;
}

size_t vertex::size() const
{
    return m_outgoing_edges.size();
}

const edge &vertex::longest_edge() const
{
    return m_outgoing_edges[m_iterator];
}

