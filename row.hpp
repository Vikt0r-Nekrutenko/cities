#ifndef ROW_HPP
#define ROW_HPP

#include "vertex.hpp"

class row
{
public:
    row();
    vertex &operator[](uint8_t indx);
    void print();
    void sort();
    void restore();

private:
    std::vector<vertex> m_columns;
};

#endif // ROW_HPP
