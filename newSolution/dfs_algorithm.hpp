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

        if(isEnd) {
            if(pathPairs.back().first.size() == 1)
                break;
            std::vector<Edge *> tmpPath {pathPairs.back().first.begin(), pathPairs.back().first.end() - 1};
            size_t tmpLength = pathPairs.back().second - pathPairs.back().first.back()->word->length();
            pathPairs.push_back({tmpPath, tmpLength});
            vertexNumber = tmpPath.back()->word->back() - 'a';
            continue;
        }

        vertexNumber = selectedEdge->word->back() - 'a';
        selectedEdge->isPassed = true;
        pathPairs.back().first.push_back(selectedEdge);
        pathPairs.back().second += selectedEdge->word->length();
    }
    for(auto &pathPair : pathPairs) {
        if(bestLength < pathPair.second) {
            bestLength = pathPair.second;
            bestPath = pathPair.first;
            std::cout << pathPairs.size() << ":" << bestPath.size() << " [" << bestLength << "]" << std::endl;
        }
    }

    return bestPath;
}

#endif // DFS_ALGORITHM_HPP
