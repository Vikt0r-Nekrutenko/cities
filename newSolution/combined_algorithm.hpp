#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include "ant_colony_algorithm.hpp"

struct Genome
{
    int regularAntCount;
    int eliteAntCount;
    int iterations;
    float alpha;
    float beta;
    float evaporation;
};

Path combined_algorithm(Matrix &matrix)
{
    Path bestPath;
    size_t bestLength = 0;
    int iterations = COLONY_ITERATIONS;

    while(iterations--) {
        int ants = REGULAR_ANTS_COUNT + ELITE_ANTS_COUNT;
        PathPairs colonyBestPathPairs;
        while(ants--) {
            int vertexNumber = rand() % MATRIX_SIZE;
            PathPairs antPathPairs{{{}, 0ull}};
            pair<Path, size_t> bestPathForOneAnt {{}, 0};

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
                    if(antPathPairs.back().first.size() <= 1) {
                        colonyBestPathPairs.push_back(bestPathForOneAnt);
                        break;
                    }
                    if(antPathPairs.back().second > bestPathForOneAnt.second) {
                        bestPathForOneAnt = antPathPairs.back();
                    }
                    Path tmpPath {antPathPairs.back().first.begin(), antPathPairs.back().first.end() - 1};
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
        }
        for(auto &pathPair : colonyBestPathPairs) {
            for(auto &edge : pathPair.first) {
                float newPheromone = edge->pheromone + float(pathPair.second) / Q;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    edge->pheromone = newPheromone;
            }
            if(bestLength < pathPair.second) {
                bestLength = pathPair.second;
                bestPath = pathPair.first;
                std::cout << iterations << "." << bestPath.size() << " [" << bestLength << "]" << std::endl;
            }
        }

        for(auto &row : matrix) {
            for(auto &vertex : row) {
                for(auto &edge : vertex) {
                    float newPheromone = edge.pheromone * EVAPORATION_VALUE;
                    if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                        edge.pheromone = newPheromone;
                }}}
    }

    return bestPath;
}

#endif // COMBINED_ALGORITHM_HPP
