#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>

class edge
{
public:
    edge(std::string *word = nullptr, uint16_t index = 0u, float pheromone = 0.f);

    const std::string &get_word() const;
    const float &get_pheromone() const;
    uint16_t get_index() const;
    uint8_t current_vertex() const;
    uint8_t next_vertex() const;
    bool is_loop() const;

    void evaporation();
    void set_index(uint16_t new_index);
    void set_pheromone(float new_pheromone);

private:
    std::string m_word;
    uint16_t m_index; // needed for easy access
    float m_pheromone;

    float m_evaporation_k = 0.35f; // coefficient determining the evaporation rate
};

#endif // EDGE_HPP
