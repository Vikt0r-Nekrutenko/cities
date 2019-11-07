#include "edge.hpp"

edge::edge(std::string *word, uint16_t index, float pheromone) :
    m_word(*word), m_index(index), m_pheromone(pheromone)
{

}

const std::string &edge::get_word() const
{
    return m_word;
}

uint16_t edge::get_index() const
{
    return m_index;
}

float edge::get_pheromone() const
{
    return m_pheromone;
}

uint8_t edge::current_vertex() const
{
    return m_word.front() - 65;
}

uint8_t edge::next_vertex() const
{
    return m_word.back() - 97;
}

bool edge::is_loop() const
{
    return current_vertex() == next_vertex();
}

void edge::evaporation()
{
    m_pheromone *= m_evaporation_k;
}

void edge::set_index(uint16_t new_index)
{
    m_index = new_index;
}

void edge::set_pheromone(float new_pheromone)
{
    m_pheromone = new_pheromone;
}

