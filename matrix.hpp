#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "row.hpp"

class matrix
{
public:
    matrix(std::vector<std::string> &strs);
    row &operator[](uint8_t indx);
    void print();
    void restore();
    void sort();
    void evaporation();

private:

    std::vector<row> m_rows;
};

#endif // MATRIX_HPP
