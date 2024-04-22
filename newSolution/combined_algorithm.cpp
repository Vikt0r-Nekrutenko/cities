#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include "combined_algorithm.hpp"

Path combined_algorithm(Matrix2d &matrix, const size_t edgeCount, const Genome &genome)
{
    for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
            Edge *ptr = matrix[x].second.data();
            Edge *end = matrix[x].second.data() + matrix[x].second.size();
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
        size_t colonyBestLength = 0;
        int colonyPathsIndex = 0;
        int colonyMaxPathIndex = 0;
        int bestPathIndx = -1;
        int ants = genome.regularAntCount + genome.greedyAntCount;
        PathPair colonyPathsPairs[genome.regularAntCount + genome.greedyAntCount];
        PathPair *colonyPathPairPtr = colonyPathsPairs;
        while(ants--) {
            int vertexNumber = beginVertex = (beginVertex < MATRIX_SIZE - 1) ? beginVertex + 1 : 0;
            PathPair workingStack {Path{edgeCount}, 0ull};
            Path::iterator stackIt = workingStack.first.begin();

            // auto t = chrono::high_resolution_clock::now();
            while(true) {
                bool isEnd = true;
                float totalProbability = 0.f;
                if(matrix[vertexNumber].first) {
                    Edge *ptr = matrix[vertexNumber].second.data();
                    Edge *end = matrix[vertexNumber].second.data() + matrix[vertexNumber].second.size();
                    while(ptr != end) {
                        Edge &edge = *ptr++;
                        if(edge.isPassed)
                            continue;
                        totalProbability += edge.prob;
                        isEnd = false;
                    }
                }

                if(isEnd) {
                    matrix[vertexNumber].first = false;
                    if(workingStack.second > colonyPathPairPtr->second) {
                        // new best path for current ant
                        *colonyPathPairPtr = {{workingStack.first.begin(), stackIt}, workingStack.second};
                    }
                    if(stackIt == workingStack.first.begin() + 1) {
                        if(bestPathPair.second < colonyPathPairPtr->second) {
                            // new global best path(index in current colony paths)
                            bestPathPair.second = colonyPathPairPtr->second;
                            bestPathIndx = colonyPathsIndex;
                        }
                        if(colonyBestLength < colonyPathPairPtr->second) {
                            // max path in current colony for local ants
                            colonyBestLength = colonyPathPairPtr->second;
                            colonyMaxPathIndex = colonyPathsIndex;
                        }
                        ++colonyPathPairPtr;
                        ++colonyPathsIndex;
                        break;
                    }
                    --stackIt;
                    workingStack.second -= (*stackIt)->word->length();
                    vertexNumber = (*stackIt)->word->front() - 'A';
                    continue;
                }

                Edge *selectedEdge = nullptr;
                float maxProbability = 0.f;
                float currentProbability = 0.f;
                float target = randf(0.00001f, 0.99999f);

                Edge *ptr = matrix[vertexNumber].second.data();
                Edge *end = matrix[vertexNumber].second.data() + matrix[vertexNumber].second.size();
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
                *stackIt++ = selectedEdge;
                workingStack.second += selectedEdge->word->length();
            }
            // tavg += float(chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - t).count()); tcount++;

            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                matrix[x].first = true;
                Edge *ptr = matrix[x].second.data();
                Edge *end = matrix[x].second.data() + matrix[x].second.size();
                while(ptr != end) {
                    ptr->isPassed = false;
                    ++ptr;
                }}
        }

        if(bestPathIndx != -1) {
            bestPathPair = colonyPathsPairs[bestPathIndx];
            cout << iterations << " " << bestPathPair.second << endl;
            ofstream mtxFile("matrixes/" + to_string(bestPathPair.second) + ".txt", ios::trunc);
            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                Edge *ptr = matrix[x].second.data();
                Edge *end = matrix[x].second.data() + matrix[x].second.size();
                while(ptr != end) {
                    mtxFile << ptr->pheromone << " ";
                    ++ptr;
                }}
            mtxFile.close();
        }


        for(int i = genome.regularAntCount + genome.greedyAntCount - 1; i >= 0; --i) {
            Edge **ptr = colonyPathsPairs[i].first.data();
            Edge **end = colonyPathsPairs[i].first.data() + colonyPathsPairs[i].first.size();

            float phadd = float(colonyPathsPairs[i].second) / Q * (i == colonyMaxPathIndex ? (float(genome.eliteAntCount) / 1.5f) : 1.f);
            while(ptr != end) {
                float newPheromone = (*ptr)->pheromone + phadd;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    (*ptr)->pheromone = newPheromone;
                ++ptr;
            }
        }

        for(int j = bestPathPair.first.size() - 1; j >= 0; --j) {
            float newPheromone = bestPathPair.first[j]->pheromone + float(bestPathPair.second) / Q * (float(genome.eliteAntCount) / 3.f);
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                bestPathPair.first[j]->pheromone = newPheromone;
        }

        for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
            Edge *ptr = matrix[x].second.data();
            Edge *end = matrix[x].second.data() + matrix[x].second.size();
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

    return bestPathPair.first;
}
