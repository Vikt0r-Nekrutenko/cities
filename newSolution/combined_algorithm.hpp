#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

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
    float prob = 0.f;
    bool isPassed = false;
};

struct Vertex {
    Edge edges[MATRIX_SIZE * MATRIX_SIZE];
    Edge *endPtr = edges;
};

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
            Edge *ptr = matrix[x][y].data();
            Edge *end = matrix[x][y].data() + matrix[x][y].size();
            while(ptr != end) {
                ptr->etha = std::pow(ptr->word->length(), genome.beta);
                ptr->prob = ptr->etha * std::pow(ptr->pheromone, genome.alpha);
                ++ptr;
            }}}

    pair<Path, size_t> bestPathPair;
    int iterations = genome.iterations;
    int beginVertex = 0;
    // float tavg = 0.f;
    // int tcount = 0;

    while(iterations--) {
        int ants = genome.regularAntCount + genome.greedyAntCount;
        size_t colonyBestLength = 0;
        int colonyPathsIndex = 0;
        int colonyMaxPathIndex = 0;
        PathPair colonyBestPathPairs[ants];
        PathPair *colonyPathPairPtr = colonyBestPathPairs;
        while(ants--) {
            int vertexNumber = beginVertex = (beginVertex < MATRIX_SIZE - 1) ? beginVertex + 1 : 0;
            PathPairs antPathPairs{{{}, 0ull}};
            size_t bestPathForOneAntIndex = 0ull;
            size_t bestPathForOneAntLength = 0ull;

            // auto t = chrono::high_resolution_clock::now();
            while(true) {
                bool isEnd = true;
                float totalProbability = 0.f;
                for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                    auto ptr = matrix[vertexNumber][y].begin();
                    auto end = matrix[vertexNumber][y].cend();
                    while(ptr != end) {
                        Edge &edge = *ptr++;
                        if(edge.isPassed)
                            continue;
                        totalProbability += edge.prob;
                        isEnd = false;
                    }}

                if(isEnd) {
                    if(antPathPairs.back().first.size() <= 1) {
                        *colonyPathPairPtr = antPathPairs[bestPathForOneAntIndex];
                        if(colonyBestLength < colonyPathPairPtr->second) {
                            colonyBestLength = colonyPathPairPtr->second;
                            colonyMaxPathIndex = colonyPathsIndex;
                        }
                        ++colonyPathPairPtr;
                        ++colonyPathsIndex;
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
                    auto ptr = matrix[vertexNumber][y].begin();
                    auto end = matrix[vertexNumber][y].cend();
                    while(ptr != end) {
                        Edge &edge = *ptr++;
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
            // tavg += float(chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - t).count()); tcount++;

            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                    Edge *ptr = matrix[x][y].data();
                    Edge *end = matrix[x][y].data() + matrix[x][y].size();
                    while(ptr != end) {
                        ptr->isPassed = false;
                        ++ptr;
                    }}}
        }

        for(int i = genome.regularAntCount + genome.greedyAntCount - 1; i >= 0; --i) {
            Edge **ptr = colonyBestPathPairs[i].first.data();
            Edge **end = colonyBestPathPairs[i].first.data() + colonyBestPathPairs[i].first.size();

            float phadd = float(colonyBestPathPairs[i].second) / Q * ((i == colonyMaxPathIndex) ? (float(genome.eliteAntCount) / 1.5f) : 1);
            while(ptr != end) {
                float newPheromone = (*ptr)->pheromone + phadd;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    (*ptr)->pheromone = newPheromone;
                ++ptr;
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
        }

        for(int j = bestPathPair.first.size() - 1; j >= 0; --j) {
            float newPheromone = bestPathPair.first[j]->pheromone + float(bestPathPair.second) / Q * (float(genome.eliteAntCount) / 3.f);
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                bestPathPair.first[j]->pheromone = newPheromone;
        }

        for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
            for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                Edge *ptr = matrix[x][y].data();
                Edge *end = matrix[x][y].data() + matrix[x][y].size();
                while(ptr != end) {
                    float newPheromone = ptr->pheromone * genome.evaporation;
                    if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                        ptr->pheromone = newPheromone;
                    ptr->prob = ptr->etha * std::pow(ptr->pheromone, genome.alpha);
                    ++ptr;
                }}}
    }
    cout << (tavg / tcount) << endl
        << (tavg / tcount) * genome.iterations * (genome.regularAntCount + genome.greedyAntCount) << endl;

    return bestPathPair;
}

#endif // COMBINED_ALGORITHM_HPP
