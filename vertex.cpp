#include "vertex.hpp"
#include <iostream>

void vertex::print()
{
    for(auto i : m_outgoing_edges){
        std::cout << i.get_index() << " " << i.get_word() << " " << i.get_pheromone() << std::endl;
    }
}

void vertex::sort()
{
    uint16_t index = 0u;
    for(std::vector<edge>::iterator it = m_outgoing_edges.begin(); it != m_outgoing_edges.end() - 1; it++){
        for(std::vector<edge>::iterator sub_it = it + 1; sub_it != m_outgoing_edges.end(); sub_it++)
            if(it->get_word().length() > sub_it->get_word().length()) {
                std::swap(*it, *sub_it);
            }
        it->set_index(index++);
    }
    m_outgoing_edges.back().set_index(index);
}

void vertex::add_edge(edge ed)
{
    m_outgoing_edges.push_back(ed);
}

bool vertex::empty() const
{
    return m_outgoing_edges.empty() ? true : false;
}

size_t vertex::size() const
{
    return m_outgoing_edges.size();
}

