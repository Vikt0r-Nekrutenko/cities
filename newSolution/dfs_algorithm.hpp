#ifndef DFS_ALGORITHM_HPP
#define DFS_ALGORITHM_HPP

#include "ant_colony_algorithm.hpp"

Path dfs_algorithm(Matrix &matrix)
{
    PathPairs pathPairs;
    Path bestPath;
    size_t bestLength = 0;
    int iterations = COLONY_ITERATIONS;
    while(iterations--) {
        int vertexNumber = rand() % MATRIX_SIZE;
        pathPairs.push_back({{}, 0ull});

        while(true) {
            Edge *selectedEdge = nullptr;
            size_t maxEdgeLength = 0ull;
            for(auto &vertex : matrix.at(vertexNumber)) {
                for(auto &edge : vertex) {
                    if(!edge.isPassed && edge.word->length() > maxEdgeLength) {
                        maxEdgeLength = edge.word->length();
                        selectedEdge = &edge;
                    }}}

            if(selectedEdge == nullptr) {
                if(pathPairs.back().first.size() == 1)
                    break;
                if(bestLength < pathPairs.back().second) {
                    bestLength = pathPairs.back().second;
                    bestPath = pathPairs.back().first;
                    std::cout << pathPairs.size() << ":" << bestPath.size() << " [" << bestLength << "]" << std::endl;
                }
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
        for(auto &pathPair : pathPairs)
            for(auto &edge : pathPair.first)
                edge->isPassed = false;
        pathPairs.clear();
    }

    return bestPath;
}

#endif // DFS_ALGORITHM_HPP
