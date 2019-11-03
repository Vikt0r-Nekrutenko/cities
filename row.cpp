#include "row.hpp"
#include <algorithm>
#include <iostream>

void row::print() const {
    for_each(begin(), end(), [](const vertex &vrtx){
        std::cout << vrtx.size() << (vrtx.size() > 9 ? " " : "  ");
    });
}

void row::sort() {
    for_each(begin(), end(), [](vertex &vrtx){
        if(!vrtx.empty())
            vrtx.sort();
    });
}
