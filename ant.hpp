#ifndef ANT_HPP
#define ANT_HPP

#include "matrix.hpp"
#include "path.hpp"

class ant
{
public:
    ant(matrix &map, uint8_t location);
    virtual ~ant() { }
    virtual void move();

    vertex *step_forward();
    void update_pheromone();

    bool can_move() const;
    const path &passed_path() const;

protected:
    virtual vertex *choose_next_vertex();

    matrix *m_global_map;
    matrix m_personal_map;
    vertex *m_location;
    path   m_passed_path;

private:
    float calculate_summary_probability(uint8_t nxt);
};

#endif // ANT_HPP
