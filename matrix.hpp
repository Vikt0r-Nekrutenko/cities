#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "row.hpp"

class matrix : public array<row, MATRIX_SIZE>
{
public:
    matrix(vector<string> source_list);
    void print() const;
    void sort();
    void evaporation();
};

#endif // MATRIX_HPP
