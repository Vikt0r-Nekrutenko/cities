#include "elite.hpp"

elite::elite(matrix &map, uint8_t location, size_t max_length_path) : ant(map, location, max_length_path)
{

}

vertex *elite::choose_next_vertex()
{
    uint8_t next_location = m_passed_path.last_edge()->next_vertex();

    vertex *highest_vrtx = nullptr;
    float max_tao = 0.f;
    for(vertex &i : m_personal_map[next_location])
        if(!i.empty() && i.longest_edge().get_pheromone() > max_tao){
            highest_vrtx = &i;
            max_tao = i.longest_edge().get_pheromone();
        }
    return highest_vrtx;
}
