#ifndef ANT_HPP
#define ANT_HPP

#include "matrix.hpp"
#include "path.hpp"

class ant
{
public:
    ant(matrix *map, uint8_t location = 0);
    ant &operator=(const ant &a);

    virtual void step_forward();
    virtual void update_pheromone();
    virtual vertex *choose_next_vertex();

    void set_path(const path &p);
    bool can_move() const;
    const path &get_path() const;
protected:
    matrix personal_map;
    path   m_path;
    matrix *global_map;
    vertex *m_location;

private:
    float calculate_summary_probability(uint8_t next_location);

    const float alpha = 1.f;
    const float beta  = 1.f;
};

#endif // ANT_HPP
