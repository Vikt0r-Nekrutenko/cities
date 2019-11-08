#include "ant.hpp"
#include "roulette.hpp"
#include <algorithm>

ant::ant(matrix &map, edge &start_location) :
    m_global_map(&map),
    m_personal_map(map),
    m_location(&m_personal_map[start_location.next_vertex()][0]) // every time starting from the edge ending "a"
{
    m_passed_path.add_passed_edge(start_location);
}

void ant::move()
{
    while(can_move())
        m_location = step_forward();
}

bool ant::can_move() const
{
    return m_location == nullptr ? false : true;
}

const path &ant::passed_path() const
{
    return m_passed_path;
}

vertex *ant::step_forward()
{
    uint8_t current_vrtx = m_location->longest_edge().current_vertex();
    uint8_t next_vrtx = m_location->longest_edge().next_vertex();
    uint16_t index = m_location->longest_edge().get_index();

    m_passed_path.add_passed_edge((*m_global_map)[current_vrtx][next_vrtx][index]); // store the current edge
    m_location->rm_edge(); // remove passed edge from map

    if(!m_location->empty() && m_location->longest_edge().is_loop()) // if next edge in current vertex is loop, at first go along it
        return &m_personal_map[m_location->longest_edge().current_vertex()][m_location->longest_edge().next_vertex()];
    else
        return choose_next_vertex();
}

vertex *ant::choose_next_vertex()
{
    uint8_t next = m_passed_path.last_edge()->next_vertex();
    float summary_probability = calculate_summary_probability(next);

    roulette roul;
    for_each(m_personal_map[next].begin(), m_personal_map[next].end(), [&](vertex &vrtx){
        if(!vrtx.empty()){
            float tao = vrtx.longest_edge().get_pheromone();
            float eta = vrtx.longest_edge().get_word().length() / m_dlim;
            float probability = 100.f * (tao * eta / summary_probability); // total probability for edge

            roul.add_sector(probability, &vrtx);
        }
    });
    return roul.spin_and_win();
}

float ant::calculate_summary_probability(uint8_t nxt)
{
    float sum = 0.f;
    for_each(m_personal_map[nxt].begin(), m_personal_map[nxt].end(), [&](vertex &vrtx){
        if(!vrtx.empty()){
            float tao = vrtx.longest_edge().get_pheromone();
            float eta = vrtx.longest_edge().get_word().length() / m_dlim;
            sum += tao * eta;
        }
    });
    return sum;
}

void ant::update_pheromone()
{
    float pheromone = 0.1f - 1.f / m_passed_path.length(); // the larger the path, the more pheromone needs to be left

    for_each(m_passed_path.begin(), m_passed_path.end(), [&](edge *ed){
        ed->set_pheromone(ed->get_pheromone() + pheromone);
    });
}
