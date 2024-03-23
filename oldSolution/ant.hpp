#ifndef ANT_HPP
#define ANT_HPP

#include "matrix.hpp"
#include "path.hpp"

class ant
{
public:
    ant(matrix &map, edge &start_location);
    virtual ~ant() {}
    virtual void move();

    vertex *step_forward();
    void update_pheromone();

    bool can_move() const;
    const path &passed_path() const;

protected:
    virtual vertex *choose_next_vertex();

    matrix *m_global_map;  // for trace of pheromone
    matrix m_personal_map; // for choose of possible edge
    vertex *m_location;    // current position on map(global)
    path m_passed_path;    // for storing passed edges

private:
    float calculate_summary_probability(uint8_t nxt);

    const float m_dlim = 100.f; // for convert edge length to a number less than one
};

#endif // ANT_HPP
