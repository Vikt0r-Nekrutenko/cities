#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define DEFAULT_PHEROMONE_VALUE 10.f
#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f
#define Q                       10'000.f
#define MATRIX_SIZE             26

using namespace std;

struct alignas(32) Edge
{
    std::string *word = nullptr;
    float pheromone = DEFAULT_PHEROMONE_VALUE;
    float etha = 0.f;
    // float tao  = 0.f;
    float prob = 0.f;
    bool isPassed = false;
    // uint8_t __padding[15];
};

struct Vertex {
    Edge edges[MATRIX_SIZE * MATRIX_SIZE];
    Edge *endPtr = edges;
};

// using Vertex = std::vector<Edge>;
// using Row = std::vector<Vertex>;
using Matrix = std::vector<Edge>[MATRIX_SIZE][MATRIX_SIZE];
using Path = std::vector<Edge *>;
using PathPair = std::pair<Path, size_t>;
using PathPairs = std::vector<PathPair>;

#define randf(min, max) ((float(rand()) / float(RAND_MAX)) * (max - min) + min)
#define randd(min, max) (rand() % (max - min) + min)

struct Genome
{
    int regularAntCount;
    int eliteAntCount;
    int greedyAntCount;
    int iterations;
    float alpha;
    float beta;
    float evaporation;
};

pair<Path, size_t> combined_algorithm(Matrix &matrix, const Genome &genome)
{
    for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
        for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
            for(size_t z = 0; z < matrix[x][y].size(); ++z) {
                matrix[x][y][z].etha = std::pow(matrix[x][y][z].word->length(), genome.beta);
                matrix[x][y][z].prob = matrix[x][y][z].etha * std::pow(matrix[x][y][z].pheromone, genome.alpha);
            }}}

    pair<Path, size_t> bestPathPair;
    int iterations = genome.iterations;
    int beginVertex = 0;

    while(iterations--) {
        int ants = genome.regularAntCount + genome.greedyAntCount;
        PathPair colonyBestPathPairs[ants];
        PathPair *cbppIt = colonyBestPathPairs;
        while(ants--) {
            int vertexNumber = beginVertex = (beginVertex < MATRIX_SIZE - 1) ? beginVertex + 1 : 0;
            PathPairs antPathPairs{{{}, 0ull}};
            size_t bestPathForOneAntIndex = 0ull;
            size_t bestPathForOneAntLength = 0ull;

            while(true) {
                bool isEnd = true;
                float totalProbability = 0.f;
                for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                    auto it = matrix[vertexNumber][y].begin();
                    auto itEnd = matrix[vertexNumber][y].cend();
                    while(it != itEnd) {
                        Edge &edge = *it++;
                        if(edge.isPassed)
                            continue;
                        totalProbability += edge.prob;
                        isEnd = false;
                    }}

                if(isEnd) {
                    if(antPathPairs.back().first.size() <= 1) {
                        *cbppIt++ = antPathPairs[bestPathForOneAntIndex];
                        break;
                    }
                    if(antPathPairs.back().second > bestPathForOneAntLength) {
                        bestPathForOneAntIndex = antPathPairs.size() - 1;
                        bestPathForOneAntLength = antPathPairs.back().second;
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

                for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                    auto it = matrix[vertexNumber][y].begin();
                    auto itEnd = matrix[vertexNumber][y].cend();
                    while(it != itEnd) {
                        Edge &edge = *it++;
                        if(edge.isPassed)
                            continue;
                        float probability = edge.prob / totalProbability;
                        currentProbability += probability;
                        if(ants > genome.greedyAntCount && target <= currentProbability){
                            selectedEdge = &edge;
                            goto endSelect;
                        } else if(ants <= genome.greedyAntCount && probability > maxProbability){
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

            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                    for(int z = matrix[x][y].size() - 1; z >= 0; --z) {
                        matrix[x][y][z].isPassed = false;
                    }}}
        }

        size_t colonyBestL = 0;
        pair<Path, size_t> *colonyBestPathPairPtr = nullptr;
        for(int i = genome.regularAntCount + genome.greedyAntCount - 1; i >= 0; --i) {
            for(int j = colonyBestPathPairs[i].first.size() - 1; j >= 0; --j) {
                float newPheromone = colonyBestPathPairs[i].first[j]->pheromone + float(colonyBestPathPairs[i].second) / Q;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    colonyBestPathPairs[i].first[j]->pheromone = newPheromone;
            }
            if(bestPathPair.second < colonyBestPathPairs[i].second) {
                bestPathPair = colonyBestPathPairs[i];
                // cout << iterations << " " << bestPathPair.second << endl;
                // ofstream mtxFile("matrixes/" + to_string(bestPathPair.second) + ".txt");
                // for(int x = MATRIX_SIZE - 1; x >= 0; --x)
                //     for(int y = MATRIX_SIZE - 1; y >= 0; --y)
                //         for(int z = matrix[x][y].size() - 1; z >= 0; --z)
                //             mtxFile << matrix[x][y][z].pheromone << " ";
                // mtxFile.close();
            }
            if(colonyBestL < colonyBestPathPairs[i].second) {
                colonyBestL = colonyBestPathPairs[i].second;
                colonyBestPathPairPtr = &colonyBestPathPairs[i];
            }
        }

        for(int j = bestPathPair.first.size() - 1; j >= 0; --j) {
            float newPheromone = bestPathPair.first[j]->pheromone + float(bestPathPair.second) / Q * (float(genome.eliteAntCount) / 3.f);
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                bestPathPair.first[j]->pheromone = newPheromone;
        }

        for(int i = colonyBestPathPairPtr->first.size() - 1; i >= 0; --i) {
            float newPheromone = colonyBestPathPairPtr->first[i]->pheromone + float(colonyBestPathPairPtr->second) / Q * (float(genome.eliteAntCount) / 1.5f);
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                colonyBestPathPairPtr->first[i]->pheromone = newPheromone;
        }

        for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
            for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                for(int z = matrix[x][y].size() - 1; z >= 0; --z) {
                    float newPheromone = matrix[x][y][z].pheromone * genome.evaporation;
                    if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                        matrix[x][y][z].pheromone = newPheromone;
                    matrix[x][y][z].prob = matrix[x][y][z].etha * std::pow(matrix[x][y][z].pheromone, genome.alpha);
                }}}
    }

    return bestPathPair;
}

#endif // COMBINED_ALGORITHM_HPP
