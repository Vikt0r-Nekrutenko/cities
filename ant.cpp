#include "ant.hpp"
#include "roulette.hpp"
#include <math.h>
#include <algorithm>

ant::ant(const matrix &map, uint8_t location, size_t max_length_path) :
    m_personal_map(map),
    m_location(&m_personal_map[location][0]),
    m_passed_path(max_length_path) { }

void ant::step_forward()
{
    m_passed_path.add_passed_edge(m_location->longest_edge());
    m_location->rm_edge();
    if(!m_location->empty() && m_location->longest_edge().is_loop())
        m_location = &m_personal_map[m_location->longest_edge().current_vertex()][m_location->longest_edge().next_vertex()];
    else
        m_location = choose_next_vertex();
}

void ant::update_pheromone()
{
    for_each(m_passed_path.begin(), m_passed_path.end(), [&](edge *ed){
        ed->set_pheromone(ed->get_pheromone() + 0.1f - 1.f / m_passed_path.length());
    });
}

bool ant::can_move() const
{
    return m_location == nullptr ? false : true;
}

const path &ant::passed_path() const
{
    return m_passed_path;
}

vertex *ant::choose_next_vertex()
{
    uint8_t nxt = m_passed_path.last_edge()->next_vertex();
    float summary_probability = calculate_summary_probability(nxt);

    roulette roul;
    for_each(m_personal_map[nxt].begin(), m_personal_map[nxt].end(), [&](vertex &vrtx){
        if(!vrtx.empty()){
            float tao = pow(vrtx.longest_edge().get_pheromone(), beta);
            float eta = pow(vrtx.longest_edge().get_word().length() / 100.f, alpha);
            float p = 100.f * (tao * eta / summary_probability);

            roul.add_sector(p, &vrtx);
        }
    });
    return roul.spin_and_win();
}

float ant::calculate_summary_probability(uint8_t nxt)
{
    float sum = 0.f;
    for_each(m_personal_map[nxt].begin(), m_personal_map[nxt].end(), [&](vertex &vrtx){
        if(!vrtx.empty()){
            float tao = pow(vrtx.longest_edge().get_pheromone(), beta);
            float eta = pow(vrtx.longest_edge().get_word().length() / 100.f, alpha);
            sum += tao * eta;
        }
    });
    return sum;
}

