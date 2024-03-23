#include "researcher.hpp"

researcher::researcher(matrix &map, edge &start_location)
    : ant(map, start_location)
{}

void researcher::move()
{
    path tmp; // for storing just longest path;
    while (can_move()) {
        m_location = step_forward();

        if (m_passed_path.length() > tmp.length())
            tmp = m_passed_path;

        while (m_location == nullptr
               && m_passed_path.length() > m_passed_path.last_edge()->get_word().length())
            m_location = step_back();
    }
    m_passed_path = tmp;
}

vertex *researcher::step_back()
{
    m_passed_path.rm_last_edge();
    return choose_next_vertex();
}
