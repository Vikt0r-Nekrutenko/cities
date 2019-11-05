#include "row.hpp"
#include <iostream>

row::row() : m_columns(26u) { }

vertex &row::operator[](uint8_t indx)
{
    return m_columns[indx];
}

void row::print()
{
    for(std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        std::cout << col->size() << (col->size() > 9 ? " " : "  ");
}

void row::sort()
{
    for(std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        if(col->size())
            col->sort();
}

void row::restore()
{
    for(std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        if(col->size())
            col->restore();
}
