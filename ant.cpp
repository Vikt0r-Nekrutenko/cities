#include "ant.hpp"
#include "roulette.hpp"
#include <algorithm>

ant::ant(matrix &map, uint8_t location) :
    m_global_map(&map),
    m_personal_map(map),
    m_location(&m_personal_map[location][0]) { }

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
    m_passed_path.add_passed_edge(m_location->longest_edge());
    m_location->rm_edge();
    if(!m_location->empty() && m_location->longest_edge().is_loop())
        return &m_personal_map[m_location->longest_edge().current_vertex()][m_location->longest_edge().next_vertex()];
    else
        return choose_next_vertex();
}

vertex *ant::choose_next_vertex()
{
    uint8_t nxt = m_passed_path.last_edge()->next_vertex();
    float summary_probability = calculate_summary_probability(nxt);

    roulette roul;
    for_each(m_personal_map[nxt].begin(), m_personal_map[nxt].end(), [&](vertex &vrtx){
        if(!vrtx.empty()){
            float tao = vrtx.longest_edge().get_pheromone();
            float eta = vrtx.longest_edge().get_word().length() / 100.f;
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
            float tao = vrtx.longest_edge().get_pheromone();
            float eta = vrtx.longest_edge().get_word().length() / 100.f;
            sum += tao * eta;
        }
    });
    return sum;
}

void ant::update_pheromone()
{
    float pheromone = 0.1f - 1.f / m_passed_path.length();

    for_each(m_passed_path.begin(), m_passed_path.end(), [&](edge *ed){
        edge &global_edge = (*m_global_map)[ed->current_vertex()][ed->next_vertex()][ed->get_index()];
        float new_pheromone = global_edge.get_pheromone() + pheromone;

        global_edge.set_pheromone(new_pheromone);
        ed->set_pheromone(new_pheromone);
    });
}
