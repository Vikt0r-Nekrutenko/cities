#include "colony.hpp"
#include <iostream>

colony::colony(std::vector<std::string> &cities_list, uint16_t colony_size) :
    m_start_edge(cities_list.back()),
    m_mtrx(cities_list, m_map_size),
    m_longest_path_length(m_start_edge.length()),
    m_colony_size(colony_size)
{
    m_mtrx.sort();
}

const std::vector<std::string> &colony::find_longest_path(size_t iter)
{
    while(iter-- > 0)
    {
        explorer explorer_ant(m_mtrx, m_start_edge.back() - 97);

        explorer_ant.move();
        explorer_ant.update_pheromone();

        try_change_longest_path(explorer_ant);

        if(iter % m_colony_size == 0)
        {
            elite elite_ant(m_mtrx, m_start_edge.back() - 97);
            elite_ant.move();
            elite_ant.update_pheromone();

            try_change_longest_path(elite_ant);

            m_mtrx.evaporation();
        }
    }

    m_longest_path.insert(m_longest_path.begin(), m_start_edge);
    return m_longest_path;
}

void colony::try_change_longest_path(const ant &a)
{
    if(a.passed_path().length() > m_longest_path_length)
    {
        m_longest_path = a.passed_path().get_words();
        m_longest_path_length = a.passed_path().length() + m_start_edge.length();

        std::cout << m_longest_path_length << " ";
    }
}
