#ifndef ELITE_HPP
#define ELITE_HPP

#include "ant.hpp"

class elite : public ant
{
public:
    elite(matrix &map, uint8_t location, size_t max_length_path);
    vertex *choose_next_vertex() override;
};

#endif // ELITE_HPP
