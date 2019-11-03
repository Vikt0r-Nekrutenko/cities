#include "matrix.hpp"
#include <algorithm>
#include <iostream>

matrix::matrix(vector<string> source_list) {
    for_each(source_list.begin(), source_list.end(), [&](const string &word){
//        if(at(word.front() - 65).at(word.back() - 97).empty())
        at(word.front() - 65).at(word.back() - 97).push_back({ word,
//                                                               uint16_t(at(word.front() - 65).at(word.back() - 97).size()),
                                                               0U,
                                                               0.1f
                                                             });
    });
}

void matrix::print() const {
    for_each(begin(), end(), [](const row &rw){
        rw.print();
        std::cout << std::endl;
    });
}

void matrix::sort() {
    for_each(begin(), end(), [](row &rw){
        rw.sort();
    });
}

void matrix::evaporation()
{
    for_each(begin(), end(), [](row &rw){
        for_each(rw.begin(), rw.end(), [](vertex &vrtx){
            for_each(vrtx.begin(), vrtx.end(), [](edge_t &edge){
                edge.pheromone *= 0.8f;
            });
        });
    });
}
