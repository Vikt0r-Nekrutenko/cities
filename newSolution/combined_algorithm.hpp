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
#define GLOBAL_ELITE_ANT_COUNT  4.f
#define LOCAL_ELITE_ANT_COUNT   9.f
#define ALPHA       2.25f
#define BETA        0.75f
#define ITERATIONS  30'000
#define EVAPORATION 0.3f

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

Path combined_algorithm(Matrix2d &matrix, const size_t edgeCount, const PathPair &prevPath = {});

void pullPheromonesIntoMatrix(Matrix2d &matrix, const string &fileName);

#endif // COMBINED_ALGORITHM_HPP
