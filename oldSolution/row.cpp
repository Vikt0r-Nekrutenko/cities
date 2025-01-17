#include "row.hpp"

row::row(uint8_t size)
    : m_columns(size)
{}

vertex &row::operator[](uint8_t indx)
{
    return m_columns[indx];
}

std::vector<vertex>::iterator row::begin()
{
    return m_columns.begin();
}

std::vector<vertex>::iterator row::end()
{
    return m_columns.end();
}

void row::sort()
{
    for (std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        if (col->size())
            col->sort();
}

void row::restore()
{
    for (std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        if (col->size())
            col->restore();
}

void row::evaporation()
{
    for (std::vector<vertex>::iterator col = m_columns.begin(); col != m_columns.end(); col++)
        if (col->size())
            col->evaporation();
}
