#include <cmath>
#include <iostream>
#include <fstream>

#include "combined_algorithm.hpp"

Path combined_algorithm(Matrix2d &matrix, const size_t edgeCount)
{
    int iterations = ITERATIONS;
    int iterToReload = ITERATIONS_TO_RELOAD_BETA;
    float localBeta = LOW_BETA;

    pair<Path, size_t> bestPathPair;
    size_t bestLength = 0;

    int lastBestIteration = ITERATIONS;
    int maxDiffWithBestIterations = 0;

    while(iterations--) {
        int colonyPathsIndex = 0;
        int bestPathIndx = -1;
        constexpr int ColonySize = REGULAR_ANT_COUNT;
        int antNumber = ColonySize;
        int vertexNumber = -1;

        PathPair colonyPathsPairs[ColonySize];
        PathPair *colonyPathPairPtr = colonyPathsPairs;
        while(antNumber--) {
            vertexNumber = (vertexNumber < MATRIX_SIZE - 1) ? vertexNumber + 1 : 0;
            PathPair workingStack {Path{edgeCount}, 0ull};
            Path::iterator stackIt = workingStack.first.begin();

            while(true) {
                bool isEnd = true;
                float totalProbability = 0.f;
                if(matrix[vertexNumber].first) {
                    Edge *ptr = matrix[vertexNumber].second.data();
                    const Edge * const end = matrix[vertexNumber].second.data() + matrix[vertexNumber].second.size();
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
                        if(bestLength < colonyPathPairPtr->second) {
                            // new global best path(index in current colony paths)
                            bestLength = colonyPathPairPtr->second;
                            bestPathIndx = colonyPathsIndex;
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

                float currentProbability = 0.f;
                const float target = randf(0.00001f, 0.99999f);

                Edge *ptr = matrix[vertexNumber].second.data();
                Edge * const end = matrix[vertexNumber].second.data() + matrix[vertexNumber].second.size();
                Edge *selectedEdge = end - 1;
                while(ptr != end) {
                    Edge &edge = *ptr++;
                    if(edge.isPassed)
                        continue;
                    const float probability = edge.prob / totalProbability;
                    currentProbability += probability;
                    if(target <= currentProbability){
                        selectedEdge = &edge;
                        break;
                    }
                }

                vertexNumber = selectedEdge->word->back() - 'a';
                selectedEdge->isPassed = true;
                *stackIt++ = selectedEdge;
                workingStack.second += selectedEdge->word->length();
            }

            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                matrix[x].first = true;
                Edge *ptr = matrix[x].second.data();
                const Edge * const end = matrix[x].second.data() + matrix[x].second.size();
                while(ptr != end) {
                    ptr->isPassed = false;
                    ++ptr;
                }}
        }

        if(bestPathIndx == -1 && iterToReload == 0) {
            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                Edge *ptr = matrix[x].second.data();
                const Edge * const end = matrix[x].second.data() + matrix[x].second.size();
                while(ptr != end) {
                    ptr->etha = std::pow(ptr->word->length(), localBeta);
                    ++ptr;
                }}
            localBeta = localBeta - LOW_BETA < 0.001f ? HIGH_BETA : LOW_BETA;
            iterToReload = ITERATIONS_TO_RELOAD_BETA;
        }

        if(bestPathIndx != -1) {
            int tmpDI = lastBestIteration - iterations;
            maxDiffWithBestIterations = tmpDI > maxDiffWithBestIterations ? tmpDI : maxDiffWithBestIterations;
            lastBestIteration = iterations;

            bestPathPair = colonyPathsPairs[bestPathIndx];
            cout << "#" << ITERATIONS - iterations << " [" << bestPathPair.second << "] [" << tmpDI << "]\t" << localBeta << "\t" << maxDiffWithBestIterations << endl;

            for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
                Edge *ptr = matrix[x].second.data();
                const Edge * const end = matrix[x].second.data() + matrix[x].second.size();
                while(ptr != end) {
                    ptr->etha = std::pow(ptr->word->length(), HIGH_BETA);
                    ++ptr;
                }}
            iterToReload = ITERATIONS_TO_RELOAD_BETA;
            localBeta = LOW_BETA;
        }


        for(int i = ColonySize - 1; i >= 0; --i) {
            Edge **ptr = colonyPathsPairs[i].first.data();
            const Edge * const * const end = colonyPathsPairs[i].first.data() + colonyPathsPairs[i].first.size();

            const float phadd = float(colonyPathsPairs[i].second) / Q;
            while(ptr != end) {
                const float newPheromone = (*ptr)->pheromone + phadd;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    (*ptr)->pheromone = newPheromone;
                ++ptr;
            }
        }

        for(int j = bestPathPair.first.size() - 1; j >= 0; --j) {
            const float newPheromone = bestPathPair.first[j]->pheromone + float(bestPathPair.second) / Q * GLOBAL_ELITE_ANT_COUNT;
            if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                bestPathPair.first[j]->pheromone = newPheromone;
        }

        for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
            Edge *ptr = matrix[x].second.data();
            const Edge * const end = matrix[x].second.data() + matrix[x].second.size();
            while(ptr != end) {
                const float newPheromone = ptr->pheromone * EVAPORATION;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    ptr->pheromone = newPheromone;
                ptr->prob = ptr->etha * std::pow(ptr->pheromone, ALPHA);
                ++ptr;
            }}
        --iterToReload;
    }

    return bestPathPair.first;
}
