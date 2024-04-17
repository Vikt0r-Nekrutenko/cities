#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include "combined_algorithm.hpp"

pair<Path, size_t> combined_algorithm(Matrix2d &matrix, const Genome &genome)
{
    for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
            Edge *ptr = matrix[x].data();
            Edge *end = matrix[x].data() + matrix[x].size();
            while(ptr != end) {
                ptr->etha = std::pow(ptr->word->length(), genome.beta);
                ptr->prob = ptr->etha * std::pow(ptr->pheromone, genome.alpha);
                ++ptr;
            }}

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
                auto ptr = matrix[vertexNumber].begin();
                auto end = matrix[vertexNumber].cend();
                while(ptr != end) {
                    Edge &edge = *ptr++;
                    if(edge.isPassed)
                        continue;
                    totalProbability += edge.prob;
                    isEnd = false;
                }

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

                ptr = matrix[vertexNumber].begin();
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
                endSelect:

                vertexNumber = selectedEdge->word->back() - 'a';
                selectedEdge->isPassed = true;
                antPathPairs.back().first.push_back(selectedEdge);
                antPathPairs.back().second += selectedEdge->word->length();
            }
            // tavg += float(chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - t).count()); tcount++;

            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                Edge *ptr = matrix[x].data();
                Edge *end = matrix[x].data() + matrix[x].size();
                while(ptr != end) {
                    ptr->isPassed = false;
                    ++ptr;
                }}
        }

        for(int i = genome.regularAntCount + genome.greedyAntCount - 1; i >= 0; --i) {
            Edge **ptr = colonyBestPathPairs[i].first.data();
            Edge **end = colonyBestPathPairs[i].first.data() + colonyBestPathPairs[i].first.size();

            float phadd = float(colonyBestPathPairs[i].second) / Q * (i == colonyMaxPathIndex ? (float(genome.eliteAntCount) / 1.5f) : 1.f);
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
            Edge *ptr = matrix[x].data();
            Edge *end = matrix[x].data() + matrix[x].size();
            while(ptr != end) {
                float newPheromone = ptr->pheromone * genome.evaporation;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    ptr->pheromone = newPheromone;
                ptr->prob = ptr->etha * std::pow(ptr->pheromone, genome.alpha);
                ++ptr;
            }}
    }
    // cout << (tavg / tcount) << endl
    // << (tavg / tcount) * genome.iterations * (genome.regularAntCount + genome.greedyAntCount) << endl;

    return bestPathPair;
}
