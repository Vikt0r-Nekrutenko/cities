#include <fstream>
#include <iterator>
#include <chrono>
#include <iostream>

// #include "genetic_algorithm.hpp"
#include "combined_algorithm.hpp"

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
size_t validate_city_list(vector<string> &resultCities, const Path &path);
PathPair read_previous_result(Matrix2d &matrix);

int write_to_file(vector<string> cities_list);

int main()
{
    vector<string> available_cities = read_available_cities();

    vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

    return write_to_file(cities_list);
}

vector<string> combine_cities(vector<string> available_cities)
{
    srand(time(nullptr));

    Matrix2d matrix(MATRIX_SIZE);
    size_t edgeCount = 0;
    for(auto &city : available_cities) {
        matrix[city.front() - 'A'].first = true;
        matrix[city.front() - 'A'].second.push_back({&city});
        ++edgeCount;
    }
    pullPheromonesIntoMatrix(matrix, "matrixes/16699.txt");

    auto algoBeginTime = chrono::high_resolution_clock::now();
    auto path = combined_algorithm(matrix, edgeCount, read_previous_result(matrix));
    cout << "Elapsed time: [" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - algoBeginTime).count() / 1000.0 << "] seconds." << endl;

    vector<string> resultCities;

    try {
        cout << "\n.::CONGRATULATION!::.\nPath length: [" << validate_city_list(resultCities, path) << "] symbols." << endl;
    } catch(char const * ex) { cout << ex << endl; }

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

size_t validate_city_list(vector<string> &resultCities, const Path &path)
{
    size_t length = 0ull;

    int validationSymbol = path.front()->word->front() - 'A';
    for(const auto &edge : path) {
        if(edge->word->front() - 'A' != validationSymbol) {
            throw "\nErorr!!! Path isn't valid!!!";
        }
        validationSymbol = edge->word->back() - 'a';
        length += edge->word->length();
        resultCities.push_back(*edge->word);
    }
    return length;
}

PathPair read_previous_result(Matrix2d &matrix)
{
    string line;
    ifstream input_file("output.txt");
    vector<string> available_cities;
    PathPair prevResult;
    while (getline(input_file, line)) {
        for(auto it = matrix[line.front() - 'A'].second.begin(); it != matrix[line.front() - 'A'].second.end(); ++it) {
            if(*(*it).word == line) {
                prevResult.first.push_back(&(*it));
                prevResult.second += (*it).word->length();
            }
        }
    }
    input_file.close();
    return prevResult;
}
