#include <fstream>
#include <chrono>
#include <iostream>
#include "genetic_algorithm.hpp"

pair<Path, size_t> genetics_algorithm(Matrix2d &matrix, int generationsCount, int genomesPerGeneration, bool arePushMatrix, const Genome &startGenome)
{
    vector<pair<Genome, size_t>> generationGenomes(genomesPerGeneration);

    for(auto &genome : generationGenomes) {
        if(startGenome.iterations == -1) {
            genome.first.regularAntCount    = randd(1, MAX_REGULAR_ANT_COUNT + 1);
            genome.first.eliteAntCount      = randd(1, MAX_ELITE_ANT_COUNT + 1);
            genome.first.greedyAntCount     = randd(1, MAX_GREEDY_ANT_COUNT + 1);
            genome.first.iterations         = randd(1, MAX_ITERATIONS + 1);
            genome.first.evaporation        = randf(0.1f, MAX_EVAPORATION);
            genome.first.alpha              = randf(0.1f, MAX_ALPHA);
            genome.first.beta               = randf(0.1f, MAX_BETA);
        } else {
            genome.first.regularAntCount    = startGenome.regularAntCount;
            genome.first.eliteAntCount      = startGenome.eliteAntCount;
            genome.first.greedyAntCount     = startGenome.greedyAntCount;
            genome.first.iterations         = startGenome.iterations;
            genome.first.evaporation        = startGenome.evaporation;
            genome.first.alpha              = startGenome.alpha;
            genome.first.beta               = startGenome.beta;
        }
    }
    Genome bestGenomeInLastGeneration {0, 0, 0, 0, 0.f, 0.f, 0.f};
    pair<Path, size_t> bestPathPair {{}, 16671};
    int generationN = 0;
    ofstream logFile("logs/" + to_string(generationsCount) + "_" + to_string(genomesPerGeneration) + "_" + to_string(time(nullptr)) + ".txt");

    do {
        int mutableGenN = rand() % GEN_PER_GENOME;
        int mutableGenerationN = rand() % genomesPerGeneration;
        switch (mutableGenN) {
        case 0: generationGenomes[mutableGenerationN].first.regularAntCount = randd(1, MAX_REGULAR_ANT_COUNT + 1); break;
        case 1: generationGenomes[mutableGenerationN].first.eliteAntCount   = randd(1, MAX_ELITE_ANT_COUNT + 1); break;
        case 2: //generationGenomes[mutableGenerationN].first.iterations      = randd(1, MAX_ITERATIONS + 1); break;
        case 3: generationGenomes[mutableGenerationN].first.evaporation     = randf(0.1f, MAX_EVAPORATION); break;
        case 4: generationGenomes[mutableGenerationN].first.alpha           = randf(0.1f, MAX_ALPHA); break;
        case 5: generationGenomes[mutableGenerationN].first.beta            = randf(0.1f, MAX_BETA); break;
        case 6: generationGenomes[mutableGenerationN].first.greedyAntCount  = randf(0.1f, MAX_GREEDY_ANT_COUNT + 1); break;
        }
        size_t bestGenomePerGenerationL = 0;

        auto generationBeginTime = chrono::high_resolution_clock::now();
        for(int i = 0; i < int(generationGenomes.size()); ++i) {

            auto beginTime = chrono::high_resolution_clock::now();
            pullPheromonesIntoMatrix(matrix, "matrixes/" + to_string(bestPathPair.second) + ".txt");
            pair<Path, size_t> pathPair = combined_algorithm(matrix, generationGenomes[i].first);
            generationGenomes[i].second = pathPair.second;

            if(generationGenomes[i].second > bestGenomePerGenerationL) {
                bestGenomePerGenerationL = generationGenomes[i].second;
                bestGenomeInLastGeneration = generationGenomes[i].first;
            }
            if(pathPair.second > bestPathPair.second) {
                bestPathPair = pathPair;
                if(arePushMatrix)
                    pushPheromonesIntoFile(matrix, "matrixes/" + to_string(bestPathPair.second) + ".txt");
            }

            cout << generationN << ":" << i << " " << pathPair.second << " " << double(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - beginTime).count()) / 1000.0 << " sec." << endl;
        }
        for(size_t i = 0; i < generationGenomes.size(); ++i)
            generationGenomes[i].first = bestGenomeInLastGeneration;
        cout << "Generation #: [" << generationN << "] result: [" << bestGenomePerGenerationL << "] per [" << double(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - generationBeginTime).count()) / 1000.0 << "] sec." << endl;
        logFile << generationN << " "
                << bestGenomePerGenerationL
                << " " << bestGenomeInLastGeneration.regularAntCount
                << " " << bestGenomeInLastGeneration.eliteAntCount
                << " " << bestGenomeInLastGeneration.greedyAntCount
                << " " << bestGenomeInLastGeneration.iterations
                << " " << bestGenomeInLastGeneration.alpha
                << " " << bestGenomeInLastGeneration.beta
                << " " << bestGenomeInLastGeneration.evaporation << endl;
    } while(++generationN < generationsCount);
    logFile.close();
    return bestPathPair;
}

void pullPheromonesIntoMatrix(Matrix2d &matrix, const string &fileName)
{
    ifstream mtxFile(fileName);
    for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
        Edge *ptr = matrix[x].second.data();
        Edge *end = matrix[x].second.data() + matrix[x].second.size();
        while(ptr != end) {
            mtxFile >> ptr->pheromone;
            ++ptr;
        }}
    mtxFile.close();
}

void pushPheromonesIntoFile(const Matrix2d &matrix, const string &fileName)
{
    ofstream mtxFile(fileName);
    for(int x = MATRIX_SIZE - 1; x >= 0; --x)   {
        const Edge *ptr = matrix[x].second.data();
        const Edge *end = matrix[x].second.data() + matrix[x].second.size();
        while(ptr != end) {
            mtxFile << ptr->pheromone << " ";
            ++ptr;
        }}
    mtxFile.close();
}
