#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include "ant_colony_algorithm.hpp"

Path combined_algorithm(Matrix &matrix)
{
    PathPairs antPathPairs;
    Path bestPath;
    int iterations = COLONY_ITERATIONS;
    int ants = REGULAR_ANTS_COUNT + ELITE_ANTS_COUNT;
    while(ants--) {
        size_t bestLength = 0;
        int vertexNumber = rand() % MATRIX_SIZE;
        antPathPairs.push_back({{}, 0ull});

        while(true) {
            bool isEnd = true;
            float totalProbability = 0.f;
            for(auto &vertex : matrix.at(vertexNumber)) {
                for(auto &edge : vertex) {
                    if(!edge.isPassed) {
                        totalProbability += std::pow(edge.pheromone, ALPHA) * std::pow(edge.word->length() / 100.f, BETA);
                        isEnd = false;
                    }}}

            if(isEnd) {
                if(antPathPairs.back().first.size() == 1){
                    for(auto &pathPair : antPathPairs)
                        if(bestLength < pathPair.second) {
                            bestLength = pathPair.second;
                            bestPath = pathPair.first;
                            std::cout << ants << "." << antPathPairs.size() << ":" << bestPath.size() << " [" << bestLength << "]" << std::endl;
                        }
                    break;
                }
                std::vector<Edge *> tmpPath {antPathPairs.back().first.begin(), antPathPairs.back().first.end() - 1};
                size_t tmpLength = antPathPairs.back().second - antPathPairs.back().first.back()->word->length();

                antPathPairs.push_back({tmpPath, tmpLength});
                vertexNumber = tmpPath.back()->word->back() - 'a';
                continue;
            }

            Edge *selectedEdge = nullptr;

            float maxProbability = 0.f;
            float currentProbability = 0.f;
            float target = (float(rand()) / float(RAND_MAX)) * (0.99999f - 0.00001f) + 0.00001f;

            for(auto &vertex : matrix.at(vertexNumber)) {
                for(auto &edge : vertex) {
                    if(edge.isPassed)
                        continue;
                    float probability = std::pow(edge.pheromone, ALPHA) * std::pow(float(edge.word->length()) / 100.f, BETA) / totalProbability;
                    currentProbability += probability;
                    if(ants > ELITE_ANTS_COUNT && target <= currentProbability){
                        selectedEdge = &edge;
                        goto endSelect;
                    } else if(ants <= ELITE_ANTS_COUNT && probability > maxProbability){
                        maxProbability = probability;
                        selectedEdge = &edge;
                    }
                }
            } endSelect:

            vertexNumber = selectedEdge->word->back() - 'a';
            selectedEdge->isPassed = true;
            antPathPairs.back().first.push_back(selectedEdge);
            antPathPairs.back().second += selectedEdge->word->length();
        }

        for(auto &row : matrix) {
            for(auto &vertex : row) {
                for(auto &edge : vertex) {
                    edge.isPassed = false;
                }}}
        for(auto &edge : bestPath) {
            float newPheromone = edge->pheromone + float(bestLength) / Q;
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                edge->pheromone = newPheromone;
        }
        antPathPairs.clear();
    }

    return bestPath;
}

#endif // COMBINED_ALGORITHM_HPP
