#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "row.hpp"

/* presents adjacency matrix
 * cells which stor outgoing edges */
class matrix
{
public:
    matrix(std::vector<std::string> &cities_list, uint8_t size);
    row &operator[](uint8_t indx);
    void restore();
    void sort();
    void evaporation();

private:
    std::vector<row> m_rows;
};

#endif // MATRIX_HPP
