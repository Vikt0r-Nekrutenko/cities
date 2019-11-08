#ifndef COLONY_HPP
#define COLONY_HPP

#include "matrix.hpp"
#include "researcher.hpp"
#include "elite.hpp"

class colony
{
public:
    colony(std::vector<std::string> &cities_list, uint16_t colony_size = 10u);
    /* iter - the amount of times that ants will go through the map */
    const path &find_longest_path(size_t iter = 500u);

private:
    void try_change_longest_path(const ant &a);

    const uint8_t m_map_size = 26u; // amount of letters of the latin alphabet

    edge m_start_edge;
    path m_longest_path;
    matrix m_mtrx;
    uint16_t m_colony_size;
};

#endif // COLONY_HPP
