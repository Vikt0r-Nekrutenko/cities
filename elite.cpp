#include "elite.hpp"

elite::elite(matrix *map, uint8_t location) : ant(map, location)
{

}

vertex *elite::choose_next_vertex()
{
    uint8_t next_location = m_path.back().word.back() - 97;
    vertex *highest_vrtx = nullptr;
    float max_tao = 0.f;
    for(vertex &i : personal_map[next_location])
        if(!i.empty() && i.back().pheromone > max_tao){
            highest_vrtx = &i;
            max_tao = i.back().pheromone;
        }
    return highest_vrtx;
}
