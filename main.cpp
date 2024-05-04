#include <cmath>
#include <fstream>
#include <iterator>
#include <chrono>
#include <iostream>

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

    return 0;//write_to_file(cities_list);
}

vector<string> combine_cities(vector<string> available_cities)
{
    // srand(time(nullptr));
    srand(1998);

    Matrix2d matrix(MATRIX_SIZE);
    size_t edgeCount = 0;
    for(auto &city : available_cities) {
        matrix[city.front() - 'A'].first = true;
        float pheromone = float(city.length());
        float etha = std::pow(city.length(), HIGH_BETA);
        float prob = etha * std::pow(pheromone, ALPHA);
        matrix[city.front() - 'A'].second.push_back({&city, pheromone, etha, prob});
        ++edgeCount;
    }
    // pullPheromonesIntoMatrix(matrix, "matrixes/16650.txt");
    // for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
    //     matrix[x].first = true;
    //     Edge *ptr = matrix[x].second.data();
    //     const Edge * const end = matrix[x].second.data() + matrix[x].second.size();
    //     while(ptr != end) {
    //         ptr->prob = ptr->etha * std::pow(ptr->pheromone, ALPHA);
    //         ++ptr;
    //     }}

    auto algoBeginTime = chrono::high_resolution_clock::now();
    auto path = combined_algorithm(matrix, edgeCount/*, read_previous_result(matrix)*/);
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
