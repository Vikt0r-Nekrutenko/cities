#include "colony.hpp"
#include <iostream>

colony::colony(std::vector<std::string> &cities_list, uint16_t colony_size) :
    m_start_edge(edge(&cities_list.back())),
    m_mtrx(cities_list, m_map_size),
    m_colony_size(colony_size)
{
    m_mtrx.sort();
}

const path &colony::find_longest_path(size_t iter)
{
    while(iter-- > 0)
    {
        researcher explorer_ant(m_mtrx, m_start_edge);
        explorer_ant.move();
        explorer_ant.update_pheromone();
        try_change_longest_path(explorer_ant);

        // after the ant colony passed through the map
        if(iter % m_colony_size == 0)
        {
            elite elite_ant(m_mtrx, m_start_edge);
            elite_ant.move();
            elite_ant.update_pheromone();

            try_change_longest_path(elite_ant);
            m_mtrx.evaporation(); // evaporate the pheromone of every edge
        }
    }

    return m_longest_path;
}

void colony::try_change_longest_path(const ant &a)
{
    if(a.passed_path().length() > m_longest_path.length())
    {
        m_longest_path = a.passed_path();
        std::cout << m_longest_path.length() << " ";
    }
}
