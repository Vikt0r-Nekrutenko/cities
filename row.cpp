#include "row.hpp"
#include <iostream>

row::row() :
    m_columns(26u)
{

}

vertex &row::operator[](char index)
{
    return m_columns[index - 97];
}

void row::print()
{
    for(std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        std::cout << col->size() << (col->size() > 9 ? " " : "  ");
}

void row::sort()
{
    for(std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        if(!col->empty())
            col->sort();
}
