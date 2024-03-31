#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include <fstream>
#include "combined_algorithm.hpp"

#define GEN_PER_GENOME          6
#define MAX_REGULAR_ANT_COUNT   26
#define MAX_ELITE_ANT_COUNT     3
#define MAX_ITERATIONS          30
#define MAX_ALPHA               4.f
#define MAX_BETA                4.f
#define MAX_EVAPORATION         1.f


pair<Path, size_t> genetics_algorithm(Matrix &matrix, int generationsCount, int genomesPerGeneration)
{
    vector<pair<Genome, size_t>> genomes(genomesPerGeneration);

    for(auto &genome : genomes) {
        genome.first.regularAntCount    = randd(1, MAX_REGULAR_ANT_COUNT + 1);
        genome.first.eliteAntCount      = randd(1, MAX_ELITE_ANT_COUNT + 1);
        genome.first.iterations         = randd(1, MAX_ITERATIONS + 1);
        genome.first.evaporation        = randf(0.1f, MAX_EVAPORATION);
        genome.first.alpha              = randf(0.1f, MAX_ALPHA);
        genome.first.beta               = randf(0.1f, MAX_BETA);
    }
    Genome bestGenome;
    pair<Path, size_t> bestPathPair;
    int generationN = 0;

    do {
        int mutableGenN = rand() % GEN_PER_GENOME;
        int mutableGenerationN = rand() % genomesPerGeneration;
        size_t bestGenomePerGenerationL = 0;

        for(int i = 0; i < int(genomes.size()); ++i) {
            pair<Path, size_t> pathPair = combined_algorithm(matrix, genomes[i].first);
            genomes[i].second = pathPair.second;

            if(genomes[i].second > bestGenomePerGenerationL) {
                bestGenomePerGenerationL = genomes[i].second;
                bestGenome = genomes[i].first;
            }
            if(pathPair.second > bestPathPair.second)
                bestPathPair = pathPair;

            if(i == mutableGenerationN) {
                switch (mutableGenN) {
                case 0: genomes[i].first.regularAntCount = randd(1, MAX_REGULAR_ANT_COUNT + 1); break;
                case 1: genomes[i].first.eliteAntCount   = randd(1, MAX_ELITE_ANT_COUNT + 1); break;
                case 2: genomes[i].first.iterations      = randd(1, MAX_ITERATIONS + 1); break;
                case 3: genomes[i].first.evaporation     = randf(0.1f, MAX_EVAPORATION); break;
                case 4: genomes[i].first.alpha           = randf(0.1f, MAX_ALPHA); break;
                case 5: genomes[i].first.beta            = randf(0.1f, MAX_BETA); break;
                }
            }
        }
        cout << bestGenomePerGenerationL << endl;

    } while(++generationN < generationsCount);
    return bestPathPair;
}

void pullPheromonesIntoMatrix(Matrix &matrix, const string &fileName)
{
    ifstream mtxFile(fileName);
    for(int x = MATRIX_SIZE - 1; x >= 0; --x)
        for(int y = MATRIX_SIZE - 1; y >= 0; --y)
            for(int z = matrix[x][y].size() - 1; z >= 0; --z)
                mtxFile >> matrix[x][y][z].pheromone;
    mtxFile.close();
}

void pushPheromonesIntoFile(const Matrix &matrix, const string &fileName)
{
    ofstream mtxFile(fileName);
    for(int x = MATRIX_SIZE - 1; x >= 0; --x)
        for(int y = MATRIX_SIZE - 1; y >= 0; --y)
            for(int z = matrix[x][y].size() - 1; z >= 0; --z)
                mtxFile << matrix[x][y][z].pheromone << " ";
    mtxFile.close();
}

#endif // GENETIC_ALGORITHM_HPP
