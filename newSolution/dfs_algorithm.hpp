#ifndef DFS_ALGORITHM_HPP
#define DFS_ALGORITHM_HPP

#include "ant_colony_algorithm.hpp"

Path dfs_algorithm(Matrix &matrix)
{
    PathPairs antPathPairs;
    Path bestPath;
    size_t bestLength = 0;
    int vertexNumber = rand() % MATRIX_SIZE;
    antPathPairs.push_back({{}, 0ull});

    while(true) {
        bool isEnd = true;
        Edge *selectedEdge = nullptr;
        for(auto &vertex : matrix.at(vertexNumber)) {
            for(auto &edge : vertex) {
                if(!edge.isPassed) {
                    selectedEdge = &edge;
                    isEnd = false;
                    goto endSelect;
                }}} endSelect:

        if(isEnd) { break; }

        vertexNumber = selectedEdge->word->back() - 'a';
        selectedEdge->isPassed = true;
        antPathPairs.back().first.push_back(selectedEdge);
        antPathPairs.back().second += selectedEdge->word->length();
    }
    return bestPath;
}

#endif // DFS_ALGORITHM_HPP
