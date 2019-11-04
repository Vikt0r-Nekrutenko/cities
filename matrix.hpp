#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "row.hpp"

class matrix
{
public:
    matrix(std::vector<std::string> &strs);
    row &operator[](char index);
    void print();
    void sort();

private:

    std::vector<row> m_rows;
};

#endif // MATRIX_HPP
