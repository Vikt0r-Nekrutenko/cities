#ifndef COMBINED_ALGORITHM_HPP
#define COMBINED_ALGORITHM_HPP

#include <string>
#include <vector>

#define DEFAULT_PHEROMONE_VALUE 5.f
#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f
#define Q                       10'000.f
#define MATRIX_SIZE             26

using namespace std;

struct alignas(32) Edge
{
    std::string *word = nullptr;
    float pheromone = DEFAULT_PHEROMONE_VALUE;
    float etha = 0.f;
    float prob = 0.f;
    bool isPassed = false;
};

using Matrix3d = std::vector<Edge>[MATRIX_SIZE][MATRIX_SIZE];
using Matrix2d = pair<bool, std::vector<Edge>>[MATRIX_SIZE];
using Path = std::vector<Edge *>;
using PathPair = std::pair<Path, size_t>;
using PathPairs = std::vector<PathPair>;

#define randf(min, max) ((float(rand()) / float(RAND_MAX)) * (max - min) + min)
#define randd(min, max) (rand() % (max - min) + min)

struct Genome
{
    int regularAntCount;
    int eliteAntCount;
    int greedyAntCount;
    int iterations;
    float alpha;
    float beta;
    float evaporation;
};

Path combined_algorithm(Matrix2d &matrix, const size_t edgeCount, const Genome &genome);

#endif // COMBINED_ALGORITHM_HPP
