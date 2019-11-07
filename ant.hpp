#ifndef ANT_HPP
#define ANT_HPP

#include "matrix.hpp"
#include "path.hpp"

class ant
{
public:
    ant(matrix &map, uint8_t location, size_t max_length_path);
    virtual ~ant() { }
    void step_forward();
    void update_pheromone();
    void forget_path();
    virtual void move();
    bool can_move() const;
    const path &passed_path() const;

protected:
    virtual vertex *choose_next_vertex();
    float calculate_summary_probability(uint8_t nxt);

    matrix *m_global_map;
    matrix m_personal_map;
    vertex *m_location;
    path   m_passed_path;

    const float alpha = 1.f, beta = 1.f;
};

#endif // ANT_HPP
