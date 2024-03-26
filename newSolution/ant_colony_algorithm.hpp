#ifndef ANT_COLONY_ALGORITHM_HPP
#define ANT_COLONY_ALGORITHM_HPP

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#define DEFAULT_PHEROMONE_VALUE 1.f
#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f
#define EVAPORATION_VALUE       0.65f
#define ALPHA 1.f
#define BETA  2.f
#define Q     100'000.f;
#define MATRIX_SIZE         26
#define COLONY_ITERATIONS   100
#define REGULAR_ANTS_COUNT  MATRIX_SIZE
#define ELITE_ANTS_COUNT    (REGULAR_ANTS_COUNT >> 1)

using namespace std;

struct Edge
{
    std::string *word = nullptr;
    float pheromone = DEFAULT_PHEROMONE_VALUE;
    bool isPassed = false;
};

using Vertex = std::vector<Edge>;
using Row = std::vector<Vertex>;
using Matrix = std::vector<Row>;
using Path = std::vector<Edge *>;
using PathPairs = std::vector<std::pair<Path, size_t>>;

Path ants_colony_algorithm(Matrix &matrix)
{
    PathPairs antPathPairs;
    Path bestPath;
    size_t bestLength = 0;
    int iterations = COLONY_ITERATIONS;

    while(iterations--) {
        int ants = REGULAR_ANTS_COUNT + ELITE_ANTS_COUNT;

        while(ants--) {
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

                if(isEnd) { break; }

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
        }
        for(auto &pathPair : antPathPairs) {
            for(auto &edge : pathPair.first) {
                float newPheromone = edge->pheromone + float(pathPair.second) / Q;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    edge->pheromone = newPheromone;
            }
            if(bestLength < pathPair.second) {
                bestLength = pathPair.second;
                bestPath = pathPair.first;
                std::cout << iterations << "." << bestPath.size() << " " << bestLength << std::endl;
            }
        }

        for(auto &row : matrix) {
            for(auto &vertex : row) {
                for(auto &edge : vertex) {
                    float newPheromone = edge.pheromone * EVAPORATION_VALUE;
                    if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                        edge.pheromone = newPheromone;
                }}}
        antPathPairs.clear();
    }

    return bestPath;
}

#endif // ANT_COLONY_ALGORITHM_HPP
