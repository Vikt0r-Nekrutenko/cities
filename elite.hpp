#ifndef ELITE_HPP
#define ELITE_HPP

#include "ant.hpp"

class elite : public ant
{
public:
    elite(matrix &map, uint8_t location);
private:
    vertex *choose_next_vertex() final;
};

#endif // ELITE_HPP
