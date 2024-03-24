#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include "ant_colony_algorithm.hpp"

Path combined_algorithm(Matrix &matrix)
{
    PathPairs antPathPairs;
    Path bestPath;
    size_t bestLength = 0;
    int iterations = COLONY_ITERATIONS;
    int ants = 1;//REGULAR_ANTS_COUNT + ELITE_ANTS_COUNT;

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
            if(antPathPairs.back().first.size() == 1)
                break;
            if(bestLength < antPathPairs.back().second) {
                bestLength = antPathPairs.back().second;
                bestPath = antPathPairs.back().first;
                std::cout << antPathPairs.size() << ":" << bestPath.size() << " [" << bestLength << "]" << std::endl;
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

    for(auto &edge : antPathPairs.back().first) {
        edge->isPassed = false;
    }

    return bestPath;
}

#endif // COMBINED_ALGORITHM_HPP
