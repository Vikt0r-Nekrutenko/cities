#ifndef ROW_HPP
#define ROW_HPP

#include "vertex.hpp"

class row
{
public:
    row();
    vertex &operator[](char index);
    void print();
    void sort();

private:
    std::vector<vertex> m_columns;
};

#endif // ROW_HPP
