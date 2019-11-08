#ifndef ELITE_HPP
#define ELITE_HPP

#include "ant.hpp"

/* the task of elite ants: walk along the edges with the maximum amount of pheromone */
class elite : public ant
{
public:
    elite(matrix &map, edge &start_location);
private:
    vertex *choose_next_vertex() final;
};

#endif // ELITE_HPP
