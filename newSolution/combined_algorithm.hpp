#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include <string>
#include <vector>

#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f
#define Q                       10'000.f
#define MATRIX_SIZE             26
#define REGULAR_ANT_COUNT       MATRIX_SIZE
#define GLOBAL_ELITE_ANT_COUNT  7.5f
#define ALPHA           2.25f
#define HIGH_BETA       0.7325f
#define LOW_BETA        0.0325f
#define ITERATIONS      200'000
#define EVAPORATION     0.3f
#define ITERATIONS_TO_RELOAD_BETA  100

using namespace std;

struct alignas(32) Edge
{
    std::string *word = nullptr;
    float pheromone   = 0.f;
    float etha        = 0.f;
    float prob        = 0.f;
    bool isPassed     = false;
};

using Matrix2d  = std::vector<pair<bool, std::vector<Edge>>>;
using Path      = std::vector<Edge *>;
using PathPair  = std::pair<Path, size_t>;

#define randf(min, max) ((float(rand()) / float(RAND_MAX)) * (max - min) + min)
#define randd(min, max) (rand() % (max - min) + min)

Path combined_algorithm(Matrix2d &matrix, const size_t edgeCount);

#endif // COMBINED_ALGORITHM_HPP
