#include "ant.hpp"
#include "roulette.hpp"
#include <algorithm>
#include <math.h>
#include <iostream>

ant::ant(matrix *map, uint8_t location) :
    personal_map(*map),
    global_map(map),
    m_location(&personal_map[location][0])
{
    m_path = path();
}

ant &ant::operator=(const ant &a)
{
    personal_map = a.personal_map;
    m_path       = a.m_path;
    m_location   = &personal_map[a.m_location->back().word.front() - 65][0];
    return *this;
}

void ant::step_forward()
{
    m_path.add_passed_edge(m_location->back()); // store passed edge
    m_location->pop_back(); // rm passed edge
    m_location = choose_next_vertex();
}

void ant::update_pheromone()
{
    std::cout << m_path.get_pl() << " ";
    for_each(m_path.begin(), m_path.end(), [&](edge_t &edge){
        (*global_map)[edge.word.front() - 65][edge.word.back() - 97][edge.index].pheromone += 1.f - 1.f / m_path.get_pl();
//        (*global_map)[edge.word.front() - 65][edge.word.back() - 97][edge.index].pheromone += 1.f / m_path.get_pl();
    });
}

vertex *ant::choose_next_vertex()
{
    uint8_t next_location = m_path.back().word.back() - 97;
    float sum = calculate_summary_probability(next_location);

    roulette roul;
    float probability = 0.f;

    for_each(personal_map[next_location].begin(), personal_map[next_location].end(), [&](vertex &vrtx){
        if(!vrtx.empty()){
            float tao = pow(vrtx.back().pheromone, beta);
            float eta = pow(vrtx.back().word.length() / 35.f, alpha);
            probability = 100.f * ((eta * tao) / sum);
//            probability = 100.f * (1.f / vrtx.back().word.length() * vrtx.back().pheromone / sum);
//std::cout << "city: " << vrtx.back().word << "\t\t pheromone: " << vrtx.back().pheromone << "\t probability: " << probability << std::endl << std::endl;
            roul.add_sector(probability, &vrtx);
        }
    });
    return roul.spin_and_win();
}

void ant::set_path(const path &p)
{
    m_path = p;
}

bool ant::can_move() const
{
    return m_location == nullptr ? false : true;
}

const path &ant::get_path() const
{
    return m_path;
}

float ant::calculate_summary_probability(uint8_t next_location)
{
    float sum = 0.f;
    for_each(personal_map[next_location].begin(), personal_map[next_location].end(), [&](vertex &vrtx){
        if(!vrtx.empty()){
            sum += pow(vrtx.back().pheromone, beta) * pow(vrtx.back().word.length() / 35.f, alpha);
//            sum += vrtx.back().pheromone / vrtx.back().word.length();
        }
    });
    return sum;
}
