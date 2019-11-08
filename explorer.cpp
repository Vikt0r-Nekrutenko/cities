#include "explorer.hpp"

explorer::explorer(matrix &map, uint8_t location) : ant(map, location) {  }

void explorer::move()
{
    path tmp;
    while (can_move()) {
        m_location = step_forward();

        if(m_passed_path.length() > tmp.length())
            tmp = m_passed_path;

        while (m_location == nullptr && m_passed_path.length() > m_passed_path.last_edge()->get_word().length())
            m_location = step_back();
    }
    m_passed_path = tmp;
}

vertex *explorer::step_back()
{
    m_passed_path.rm_last_edge();
    return choose_next_vertex();
}
