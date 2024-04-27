#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include <string>
#include <vector>

#define DEFAULT_PHEROMONE_VALUE 5.f
#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f
#define Q                       10'000.f
#define MATRIX_SIZE             26
#define REGULAR_ANT_COUNT       MATRIX_SIZE
#define GLOBAL_ELITE_ANT_COUNT  7.5f
#define ALPHA       2.25f
/**
 * BETA = 0.87325 : 16590/20000(sym/iter)
 *  +22/5000(sym/iter)
 *  -12/2500(sym/iter)
 *  16700 -> 16694 -4/2500(sym/iter) -LPR
 *  16700 -> 16700 0/2500(sym/iter) +LPR
 *  AVG goal: 1.5p
 *
 * BETA = 0.07325 : 16569/20000(sym/iter)
 *  +2/5000(sym/iter)
 *  -7/2500(sym/iter)
 *  16700 -> 16694 -6/2500(sym/iter) -LPR
 *  16700 -> 16701 +1/2500(sym/iter) +LPR
 *  AVG goal: -2.5p
 *
 * BETA = 0.0325 : 16549/20000(sym/iter)
 *  +35/5000(sym/iter)
 *  -6/2500(sym/iter)
 *  16700 -> 16696 -4/2500(sym/iter) -LPR
 *  16700 -> 16701 +1/2500(sym/iter) +LPR
 *  AVG goal: 6.5p
*/
#define BETA        0.0325f
#define ITERATIONS  1'000
#define EVAPORATION 0.1f

using namespace std;

struct alignas(32) Edge
{
    std::string *word = nullptr;
    float pheromone = DEFAULT_PHEROMONE_VALUE;
    float etha = 0.f;
    float prob = 0.f;
    bool isPassed = false;
};

using Matrix2d  = std::vector<pair<bool, std::vector<Edge>>>;
using Path      = std::vector<Edge *>;
using PathPair  = std::pair<Path, size_t>;

#define randf(min, max) ((float(rand()) / float(RAND_MAX)) * (max - min) + min)
#define randd(min, max) (rand() % (max - min) + min)

Path combined_algorithm(Matrix2d &matrix, const size_t edgeCount, const PathPair &prevPath = {{}, 0});

void pullPheromonesIntoMatrix(Matrix2d &matrix, const string &fileName);

#endif // COMBINED_ALGORITHM_HPP
