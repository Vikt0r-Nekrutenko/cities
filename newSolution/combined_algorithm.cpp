#include <cmath>
#include <iostream>
#include <fstream>

#include "combined_algorithm.hpp"

Path combined_algorithm(Matrix2d &matrix, const size_t edgeCount, const PathPair &prevPath)
{
    for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
        Edge *ptr = matrix[x].second.data();
        const Edge * const end = matrix[x].second.data() + matrix[x].second.size();
        while(ptr != end) {
            ptr->etha = std::pow(ptr->word->length(), BETA);
            ptr->prob = ptr->etha * std::pow(ptr->pheromone, ALPHA);
            ++ptr;
        }}

    pair<Path, size_t> bestPathPair = prevPath;
    int iterations = ITERATIONS;

    while(iterations--) {
        size_t colonyBestLength = 0;
        int colonyPathsIndex = 0;
        int colonyMaxPathIndex = 0;
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
                float currentProbability = 0.f;
                const float target = randf(0.00001f, 0.99999f);

                Edge *ptr = matrix[vertexNumber].second.data();
                const Edge * const end = matrix[vertexNumber].second.data() + matrix[vertexNumber].second.size();
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

        if(bestPathIndx != -1) {
            bestPathPair = colonyPathsPairs[bestPathIndx];
            cout << "Iteration #" << iterations << " Path length: [" << bestPathPair.second << "]." << endl;
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


        for(int i = ColonySize - 1; i >= 0; --i) {
            Edge **ptr = colonyPathsPairs[i].first.data();
            const Edge * const * const end = colonyPathsPairs[i].first.data() + colonyPathsPairs[i].first.size();

            const float phadd = float(colonyPathsPairs[i].second) / Q * (i == colonyMaxPathIndex ? LOCAL_ELITE_ANT_COUNT : 1.f);
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
    }

    return bestPathPair.first;
}

void pullPheromonesIntoMatrix(Matrix2d &matrix, const string &fileName)
{
    ifstream mtxFile(fileName);
    for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
        Edge *ptr = matrix[x].second.data();
        const Edge * const end = matrix[x].second.data() + matrix[x].second.size();
        while(ptr != end) {
            mtxFile >> ptr->pheromone;
            ++ptr;
        }}
    mtxFile.close();
}
