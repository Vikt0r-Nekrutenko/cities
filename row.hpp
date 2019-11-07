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
    void evaporation();

    std::vector<vertex>::iterator begin();
    std::vector<vertex>::iterator end();

private:
    std::vector<vertex> m_columns;
};

#endif // ROW_HPP
