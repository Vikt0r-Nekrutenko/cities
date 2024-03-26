#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include "ant_colony_algorithm.hpp"

#define randf(min, max) ((float(rand()) / float(RAND_MAX)) * (max - min) + min)
#define randd(min, max) (rand() % (max - min) + min)

struct Genome
{
    int regularAntCount;
    int eliteAntCount;
    int iterations;
    float alpha;
    float beta;
    float evaporation;
};

Path combined_algorithm(Matrix &matrix, const Genome &genome)
{
    Path bestPath;
    size_t bestLength = 0;
    int iterations = genome.iterations;

    while(iterations--) {
        int ants = genome.regularAntCount + genome.eliteAntCount;
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
                            totalProbability += std::pow(edge.pheromone, genome.alpha) * std::pow(edge.word->length() / 100.f, genome.beta);
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

                    antPathPairs.push_back({{antPathPairs.back().first.begin(), antPathPairs.back().first.end() - 1},
                                            antPathPairs.back().second - antPathPairs.back().first.back()->word->length()});
                    vertexNumber = antPathPairs.back().first.back()->word->back() - 'a';
                    continue;
                }

                Edge *selectedEdge = nullptr;
                float maxProbability = 0.f;
                float currentProbability = 0.f;
                float target = randf(0.00001f, 0.99999f);

                for(auto &vertex : matrix.at(vertexNumber)) {
                    for(auto &edge : vertex) {
                        if(edge.isPassed)
                            continue;
                        float probability = std::pow(edge.pheromone, genome.alpha) * std::pow(edge.word->length() / 100.f, genome.beta) / totalProbability;
                        currentProbability += probability;
                        if(ants > genome.eliteAntCount && target <= currentProbability){
                            selectedEdge = &edge;
                            goto endSelect;
                        } else if(ants <= genome.eliteAntCount && probability > maxProbability){
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
                    float newPheromone = edge.pheromone * genome.evaporation;
                    if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                        edge.pheromone = newPheromone;
                }}}
    }

    return bestPath;
}

#endif // COMBINED_ALGORITHM_HPP
