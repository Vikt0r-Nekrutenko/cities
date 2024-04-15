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

struct Edge
{
    std::string *word = nullptr;
    float pheromone = DEFAULT_PHEROMONE_VALUE;
    bool isPassed = false;
    float etha = 0.f;
};

using Vertex = std::vector<Edge>;
using Row = std::vector<Vertex>;
using Matrix = std::vector<Row>;
using Path = std::vector<Edge *>;
using PathPairs = std::vector<std::pair<Path, size_t>>;

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
            }}}

    pair<Path, size_t> bestPath;
    size_t bestLength = 0;
    int iterations = genome.iterations;
    int beginVertex = 0;

    while(iterations--) {
        int ants = genome.regularAntCount + genome.greedyAntCount;
        PathPairs colonyBestPathPairs;
        while(ants--) {
            int vertexNumber = beginVertex = (beginVertex < MATRIX_SIZE - 1) ? beginVertex + 1 : 0;
            PathPairs antPathPairs{{{}, 0ull}};
            size_t bestPathForOneAntIndex = 0ull;
            size_t bestPathForOneAntLength = 0ull;

            while(true) {
                bool isEnd = true;
                float totalProbability = 0.f;
                for(int y = MATRIX_SIZE - 1; y >= 0; --y) {
                    for(int z = matrix[vertexNumber][y].size() - 1; z >= 0; --z) {
                        Edge &edge = matrix[vertexNumber][y][z];
                        if(!edge.isPassed) {
                            totalProbability += std::pow(edge.pheromone, genome.alpha) * edge.etha;
                            isEnd = false;
                        }}}

                if(isEnd) {
                    if(antPathPairs.back().first.size() <= 1) {
                        colonyBestPathPairs.push_back(antPathPairs[bestPathForOneAntIndex]);
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

                for(int y = 25; y >= 0; --y) {
                    for(int z = matrix[vertexNumber][y].size() - 1; z >= 0; --z) {
                        Edge &edge = matrix[vertexNumber][y][z];
                        if(edge.isPassed)
                            continue;
                        float probability = std::pow(edge.pheromone, genome.alpha) * edge.etha / totalProbability;
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
                    for(size_t z = 0; z < matrix[x][y].size(); ++z) {
                        matrix[x][y][z].isPassed = false;
                    }}}
        }

        pair<Path, size_t> colonyBestPathPair {{}, 0};
        for(int i = colonyBestPathPairs.size() - 1; i >= 0; --i) {
            for(int j = colonyBestPathPairs[i].first.size() - 1; j >= 0; --j) {
                float newPheromone = colonyBestPathPairs[i].first[j]->pheromone + float(colonyBestPathPairs[i].second) / Q;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    colonyBestPathPairs[i].first[j]->pheromone = newPheromone;
            }
            if(bestLength < colonyBestPathPairs[i].second) {
                bestLength = colonyBestPathPairs[i].second;
                bestPath = colonyBestPathPairs[i];
                cout << iterations << " " << bestLength << endl;
                // ofstream mtxFile("matrixes/" + to_string(bestLength) + ".txt");
                // for(int x = MATRIX_SIZE - 1; x >= 0; --x)
                //     for(int y = MATRIX_SIZE - 1; y >= 0; --y)
                //         for(int z = matrix[x][y].size() - 1; z >= 0; --z)
                //             mtxFile << matrix[x][y][z].pheromone << " ";
                // mtxFile.close();
            }
            if(colonyBestPathPair.second < colonyBestPathPairs[i].second)
                colonyBestPathPair = colonyBestPathPairs[i];
        }

        for(int j = bestPath.first.size() - 1; j >= 0; --j) {
            float newPheromone = bestPath.first[j]->pheromone + float(bestLength) / Q * (float(genome.eliteAntCount) / 3.f);
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                bestPath.first[j]->pheromone = newPheromone;
        }
        for(int i = colonyBestPathPair.first.size() - 1; i >= 0; --i) {
            float newPheromone = colonyBestPathPair.first[i]->pheromone + float(colonyBestPathPair.second) / Q * (float(genome.eliteAntCount) / 1.5f);
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                colonyBestPathPair.first[i]->pheromone = newPheromone;
        }

        for(auto xt = matrix.begin(); xt != matrix.end(); ++xt) {
            for(auto yt = xt->begin(); yt != xt->end(); ++yt) {
                for(auto zt = yt->begin(); zt != yt->end(); ++zt) {
                    float newPheromone = zt->pheromone * genome.evaporation;
                    if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                        zt->pheromone = newPheromone;
                }
            }
        }
    }

    return bestPath;
}

#endif // COMBINED_ALGORITHM_HPP
