#ifndef EXPLORER_HPP
#define EXPLORER_HPP

#include "ant.hpp"

class explorer : public ant
{
public:
    explorer(const matrix &map, uint8_t location, size_t max_length_path);
    ~explorer() final { }
    void move() override;
    void step_back();
};

#endif // EXPLORER_HPP
