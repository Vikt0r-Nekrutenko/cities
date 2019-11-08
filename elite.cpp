#include "elite.hpp"
#include <algorithm>

elite::elite(matrix &map, uint8_t location) : ant(map, location) { }

vertex *elite::choose_next_vertex()
{
    uint8_t next_location = m_passed_path.last_edge()->next_vertex();

    vertex *highest_vrtx = nullptr;
    float max_tao = 0.f;
    for_each(m_personal_map[next_location].begin(), m_personal_map[next_location].end(), [&](vertex &vrtx){
        if(!vrtx.empty() && vrtx.longest_edge().get_pheromone() > max_tao){
            highest_vrtx = &vrtx;
            max_tao = vrtx.longest_edge().get_pheromone();
        }
    });
    return highest_vrtx;
}
