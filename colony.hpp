#ifndef COLONY_HPP
#define COLONY_HPP

#include "matrix.hpp"
#include "explorer.hpp"
#include "elite.hpp"

#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

class colony
{
public:
    colony(std::vector<std::string> &cities_list, uint16_t colony_size = 10u);
    const std::vector<std::string> &find_longest_path(size_t iter = 500u);

private:
    void try_change_longest_path(const ant &a);

    const uint8_t m_map_size = 26u;
    const std::string &m_start_edge;

    std::vector<std::string> m_longest_path;
    matrix m_mtrx;
    size_t m_longest_path_length;
    uint16_t m_colony_size;
};

#endif // COLONY_HPP
