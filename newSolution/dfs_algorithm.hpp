#ifndef DFS_ALGORITHM_HPP
#define DFS_ALGORITHM_HPP

#include "ant_colony_algorithm.hpp"

Path dfs_algorithm(Matrix &matrix)
{
    PathPairs pathPairs;
    Path bestPath;
    size_t bestLength = 0;
    int vertexNumber = rand() % MATRIX_SIZE;
    pathPairs.push_back({{}, 0ull});

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
        pathPairs.back().first.push_back(selectedEdge);
        pathPairs.back().second += selectedEdge->word->length();
    }
    for(auto &pathPair : pathPairs) {
        if(bestLength < pathPair.second) {
            bestLength = pathPair.second;
            bestPath = pathPair.first;
            std::cout << bestPath.size() << " " << bestLength << std::endl;
        }
    }

    return bestPath;
}

#endif // DFS_ALGORITHM_HPP
