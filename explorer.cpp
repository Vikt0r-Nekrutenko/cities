#include "explorer.hpp"
#include <algorithm>
#include <iostream>

explorer::explorer(matrix &map, uint8_t location, size_t max_length_path) : ant(map, location, max_length_path) {  }

void explorer::move()
{
    path tmp(1);
    while (can_move()) {
        step_forward();

        if(m_passed_path.length() > tmp.length())
            tmp = m_passed_path;

        while (m_location == nullptr && m_passed_path.length() > m_passed_path.last_edge()->get_word().length())
            step_back();
    }
    m_passed_path = tmp;
    update_pheromone();
}

void explorer::step_back()
{
    m_passed_path.rm_last_edge();
    m_location = choose_next_vertex();
}
