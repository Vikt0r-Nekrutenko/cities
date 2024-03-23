#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#define MATRIX_SIZE 26
#define DEFAULT_PHEROMONE_VALUE 0.1f
#define MINIMUM_PHEROMONE_VALUE 0.01f
#define MAXIMUM_PHEROMONE_VALUE 100.f

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

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
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
    return available_cities;
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
