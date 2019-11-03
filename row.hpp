#ifndef ROW_HPP
#define ROW_HPP

#include "vertex.hpp"
#include <array>

using std::array;

const uint16_t MATRIX_SIZE = 26U;

class row : public array<vertex, MATRIX_SIZE>
{
public:

    void print() const;
    void sort();
};

#endif // ROW_HPP
