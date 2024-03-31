#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include "combined_algorithm.hpp"

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

    } while(++generationN < generationsCount);
}

#endif // GENETIC_ALGORITHM_HPP
