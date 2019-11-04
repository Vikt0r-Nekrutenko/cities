#include "matrix.hpp"
#include <iostream>

matrix::matrix(std::vector<std::string> &strs) :
    m_rows(26u)
{
    for(std::vector<std::string>::iterator word = strs.begin(); word != strs.end(); word++)
        (*this)[word->front()][word->back()].add_edge( edge(&*word, 0u, 0.1f) );
}

row &matrix::operator[](char index)
{
    return m_rows[index - 65];
}

void matrix::print()
{
    for(std::vector<row>::iterator row = m_rows.begin(); row != m_rows.end(); row++){
        row->print();
        std::cout << std::endl;
    }
}

void matrix::sort()
{
    for(std::vector<row>::iterator row = m_rows.begin(); row != m_rows.end(); row++)
        row->sort();
}
