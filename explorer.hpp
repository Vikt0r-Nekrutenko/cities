#ifndef EXPLORER_HPP
#define EXPLORER_HPP

#include "ant.hpp"

class explorer : public ant
{
public:
    explorer(matrix &map, uint8_t location);
    ~explorer() final { }
    void move() final;
    vertex *step_back();
};

#endif // EXPLORER_HPP
