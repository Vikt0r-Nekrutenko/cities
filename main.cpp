#include <fstream>
#include <iostream>
#include <iterator>
#include <cmath>
#include <string>
#include <vector>

#define MATRIX_SIZE 26
#define DEFAULT_PHEROMONE_VALUE 0.1f
#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f
#define ALPHA 1.f
#define BETA  1.f

using namespace std;

struct Edge
{
    std::string *word = nullptr;
    float pheromone = DEFAULT_PHEROMONE_VALUE;
    bool isPassed = false;
};

using Vertex = std::vector<Edge>;
using Row = std::vector<Vertex>;
using Matrix = std::vector<Row>;
using Path = std::vector<Edge *>;
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
    Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
    for(auto &city : available_cities) {
        matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
    }

    Path path = ants_colony_algorithm(matrix);

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
    int vertexNumber = 0;//rand() % MatrixWidth;
    float totalProbability = 0.f;
    for(auto &vertex : matrix.at(vertexNumber)) {
        for(auto &edge : vertex) {
            if(!edge.isPassed) {
                totalProbability += std::pow(edge.pheromone, ALPHA) * std::pow(edge.word->length() / 100.f, BETA);
            }
        }
    }
}
