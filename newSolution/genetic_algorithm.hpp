#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include "combined_algorithm.hpp"

#define GEN_PER_GENOME          7
#define MAX_REGULAR_ANT_COUNT   MATRIX_SIZE
#define MAX_ELITE_ANT_COUNT     MATRIX_SIZE * 2
#define MAX_GREEDY_ANT_COUNT    6
#define MAX_ITERATIONS          1
#define MAX_ALPHA               5.f
#define MAX_BETA                5.f
#define MAX_EVAPORATION         1.f

void pullPheromonesIntoMatrix(Matrix2d &matrix, const string &fileName);

void pushPheromonesIntoFile(const Matrix2d &matrix, const string &fileName);

pair<Path, size_t> genetics_algorithm(Matrix2d &matrix, int generationsCount, int genomesPerGeneration, bool arePushMatrix = false, const Genome &startGenome = {-1,-1,-1,-1,-1,-1,-1});

#endif // GENETIC_ALGORITHM_HPP
