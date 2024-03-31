#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

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
        genome.first.regularAntCount    = randd(1, 26);
        genome.first.eliteAntCount      = randd(1, 5);
        genome.first.iterations         = randd(1, 30);
        genome.first.evaporation        = randf(0.1f, 1.f);
        genome.first.alpha              = randf(0.1f, 4.f);
        genome.first.beta               = randf(0.1f, 4.f);
    }
    Genome bestGenome;
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

            if(i == mutableGenerationN) {
                switch (mutableGenN) {
                case 0: genomes[i].first.regularAntCount = randd(1, 27); break;
                case 1: genomes[i].first.eliteAntCount   = randd(1, 6); break;
                case 2: genomes[i].first.iterations      = randd(1, 31); break;
                case 3: genomes[i].first.evaporation     = randf(0.1f, 1.f); break;
                case 4: genomes[i].first.alpha           = randf(0.1f, 4.f); break;
                case 5: genomes[i].first.beta            = randf(0.1f, 4.f); break;
                }
            }
        }

    } while(++generationN < generationsCount);
}

#endif // GENETIC_ALGORITHM_HPP
