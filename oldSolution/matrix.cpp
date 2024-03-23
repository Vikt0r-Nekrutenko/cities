#include "matrix.hpp"

matrix::matrix(std::vector<std::string> &cities_list, uint8_t size)
    : m_rows(size)
{
    for (std::vector<row>::iterator col = m_rows.begin(); col != m_rows.end(); col++)
        *col = row(size);

    /* used cities_list.end - 1 couse last city name will be start edge */
    for (std::vector<std::string>::iterator word = cities_list.begin();
         word != cities_list.end() - 1;
         word++)
        /* word->front() - 65u == ('A' - 65 = 0)
         * word->back() - 97u == ('a' - 97 = 0)
         * for simple indexation of matrix */
        (*this)[word->front() - 65u][word->back() - 97u].add_edge(edge(&*word, 0u, 0.1f));
}

row &matrix::operator[](uint8_t indx)
{
    return m_rows[indx];
}

void matrix::restore()
{
    for (std::vector<row>::iterator row = m_rows.begin(); row != m_rows.end(); row++)
        row->restore();
}

void matrix::sort()
{
    for (std::vector<row>::iterator row = m_rows.begin(); row != m_rows.end(); row++)
        row->sort();
}

void matrix::evaporation()
{
    for (std::vector<row>::iterator row = m_rows.begin(); row != m_rows.end(); row++)
        row->evaporation();
}
