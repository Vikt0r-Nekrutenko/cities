#include <fstream>
#include <iostream>
#include <iterator>
#include <cmath>
#include <string>
#include <vector>
#include <chrono>

#define DEFAULT_PHEROMONE_VALUE 0.1f
#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f
#define EVAPORATION_VALUE       0.65f
#define ALPHA 1.f
#define BETA  1.f
#define Q     100'000.f;
#define MATRIX_SIZE         26
#define COLONY_ITERATIONS   100
#define REGULAR_ANTS_COUNT  MATRIX_SIZE
#define ELITE_ANTS_COUNT    (REGULAR_ANTS_COUNT >> 1)

using namespace std;

struct Edge
{
    std::string *word = nullptr;
    float pheromone = DEFAULT_PHEROMONE_VALUE;
    bool isPassed = false;
};

struct Sector
{
    Edge *edge;
    float begin;
    float end;
};

using Vertex = std::vector<Edge>;
using Row = std::vector<Vertex>;
using Matrix = std::vector<Row>;
using Path = std::vector<Edge *>;
using Roulette = std::vector<Sector>;
using PathPairs = std::vector<std::pair<Path, size_t>>;

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
Path ants_colony_algorithm(Matrix &matrix);

int write_to_file(vector<string> cities_list);

int main()
{
    vector<string> available_cities = read_available_cities();

    vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

    return write_to_file(cities_list);
}

vector<string> combine_cities(vector<string> available_cities)
{
    srand(1998);
    Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
    for(auto &city : available_cities) {
        matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
    }

    auto antsColonyAlgoBeginTime = chrono::high_resolution_clock::now();
    Path path = ants_colony_algorithm(matrix);
    cout << "Ant colony elapsed time: [" << chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - antsColonyAlgoBeginTime).count() << "] sec." << endl;

    size_t length = 0ull;
    vector<string> resultCities;

    int validationSymbol = path.front()->word->front() - 'A';
    for(const auto &edge : path) {
        if(edge->word->front() - 'A' != validationSymbol) {
            cout << "Erorr!!! Path isn't valid!!!" << endl;
            throw;
        }
        validationSymbol = edge->word->back() - 'a';
        length += edge->word->length();
        resultCities.push_back(*edge->word);
    }
    cout << "CONGRATULATION!!! Path length: [" << length << "] symbols." << endl;

    return resultCities;
}

vector<string> read_available_cities()
{
    string line;
    ifstream input_file("input.txt");
    vector<string> available_cities;
    while (getline(input_file, line)) {
        available_cities.push_back(line);
    }
    input_file.close();

    return available_cities;
}

int write_to_file(vector<string> cities_list)
{
    ofstream output_file("./output.txt");
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(cities_list.begin(), cities_list.end(), output_iterator);

    return 0;
}

Path ants_colony_algorithm(Matrix &matrix)
{
    PathPairs antPathPairs;
    Path bestPath;
    size_t bestLength = 0;
    int iterations = COLONY_ITERATIONS;

    while(iterations--) {
        int ants = REGULAR_ANTS_COUNT + ELITE_ANTS_COUNT;

        while(ants--) {
            int vertexNumber = 0;//rand() % MatrixWidth;
            antPathPairs.push_back({{}, 0ull});

            while(true) {
                bool isEnd = true;
                float totalProbability = 0.f;
                for(auto &vertex : matrix.at(vertexNumber)) {
                    for(auto &edge : vertex) {
                        if(!edge.isPassed) {
                            totalProbability += std::pow(edge.pheromone, ALPHA) * std::pow(edge.word->length() / 100.f, BETA);
                            isEnd = false;
                        }
                    }
                }

                if(isEnd) {
                    break;
                }

                Roulette roulette;
                float currentProbability = 0.f;
                for(auto &vertex : matrix.at(vertexNumber)) {
                    for(auto &edge : vertex) {
                        if(edge.isPassed)
                            continue;
                        float probability = std::pow(edge.pheromone, ALPHA) * std::pow(float(edge.word->length()) / 100.f, BETA) / totalProbability;
                        roulette.push_back({&edge, currentProbability, currentProbability += probability});
                    }
                }

                Edge *selectedEdge = nullptr;
                float target = float(rand()) / float(RAND_MAX);
                int low = 0;
                int high = roulette.size() - 1;

                while(low <= high) {
                    int mid = low + (high - low) / 2;
                    if(target >= roulette.at(mid).begin && target <= roulette.at(mid).end){
                        selectedEdge = roulette.at(mid).edge;
                        break;
                    } else if(target < roulette.at(mid).begin) {
                        high = mid - 1;
                    } else {
                        low = mid + 1;
                    }
                }

                vertexNumber = selectedEdge->word->back() - 'a';
                selectedEdge->isPassed = true;
                antPathPairs.back().first.push_back(selectedEdge);
                antPathPairs.back().second += selectedEdge->word->length();
            }

            for(auto &edge : antPathPairs.back().first) {
                edge->isPassed = false;
            }
        }
        for(auto &pathPair : antPathPairs) {
            for(auto &edge : pathPair.first) {
                float newPheromone = edge->pheromone + float(pathPair.second) / Q;
                if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                    edge->pheromone = newPheromone;
            }
            if(bestLength > pathPair.second) {
                bestLength = pathPair.second;
                bestPath = pathPair.first;
            }
        }

        for(auto &row : matrix) {
            for(auto &vertex : row) {
                for(auto &edge : vertex) {
                    float newPheromone = edge.pheromone * EVAPORATION_VALUE;
                    if(newPheromone >= MINIMUM_PHEROMONE_VALUE && newPheromone <= MAXIMUM_PHEROMONE_VALUE)
                        edge.pheromone = newPheromone;
                }
            }
        }
    }

    return bestPath;
}
