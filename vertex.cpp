#include "vertex.hpp"
#include <algorithm>
#include <iostream>

void vertex::print() const {
    for_each(begin(), end(), [](const edge_t &edge){
        std::cout << edge.index << " " << edge.word << " " << edge.pheromone << std::endl;
    });
}

void vertex::sort() {
    uint16_t index = 0;
    for(vertex::iterator it = begin(); it != end() - 1; it++){
        for(vertex::iterator sub_it = it + 1; sub_it != end(); sub_it++)
            if(it->word.length() > sub_it->word.length())
                std::swap(*it, *sub_it);
        it->index = index++;
    }
    back().index = index;
}
