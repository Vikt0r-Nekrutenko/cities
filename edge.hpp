#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>

class edge
{
public:
    edge(std::string *word = nullptr, uint16_t index = 0u, float pheromone = 0.f);

    const std::string &get_word() const;
    uint16_t get_index() const;
    float get_pheromone() const;

    uint8_t current_vertex() const;
    uint8_t next_vertex() const;

    void set_index(uint16_t new_index);
    void set_pheromone(float new_pheromone);

private:
    std::string m_word;
    uint16_t m_index;
    float m_pheromone;
};

#endif // EDGE_HPP
