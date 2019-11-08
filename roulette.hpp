#ifndef ROULETTE_HPP
#define ROULETTE_HPP

#include "vertex.hpp"
#include <random>

class roulette
{
public:
    void add_sector(float pobability, vertex *vrtx);
    vertex *spin_and_win();
private:
    std::vector<std::pair<vertex *, float>> m_wheel;
    float total_probability;
    static std::mt19937 gen;
};

#endif // ROULETTE_HPP
