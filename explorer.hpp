#ifndef EXPLORER_HPP
#define EXPLORER_HPP

#include "ant.hpp"

class explorer : public ant
{
public:
    explorer(matrix *map, uint8_t location = 0);
    vertex *choose_next_vertex() override;
};

#endif // EXPLORER_HPP
