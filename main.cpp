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
    srand(1998);
    // srand(time(nullptr));

    Matrix2d matrix(MATRIX_SIZE);
    size_t edgeCount = 0;
    for(auto &city : available_cities) {
        // if(mtx[city.front() - 'A'][city.back() - 'a'].empty()) {
        // if(matrix[city.front() - 'A'].edges[city.back() - 'a'].word == nullptr)
            matrix[city.front() - 'A'].first = true;
            matrix[city.front() - 'A'].second.push_back({&city});
            ++edgeCount;
        // }
    }
    // auto time1 = chrono::high_resolution_clock::now();
    // cout << "time1: [" << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - time1).count() << "] min." << endl;
    // return available_cities;

    // 17:39 - 19:53 [2:13] = 16595/10000
    // 20:51 - 21:41 [1:00] = 16564/1500?
    // 1800
    ifstream mtxFile("matrixes/16699.txt");
    for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
        Edge *ptr = matrix[x].second.data();
        Edge *end = matrix[x].second.data() + matrix[x].second.size();
        while(ptr != end) {
            mtxFile >> ptr->pheromone;
            ++ptr;
        }}
    mtxFile.close();

    auto geneticsAlgoBeginTime = chrono::high_resolution_clock::now();
    // // auto path = genetics_algorithm(matrix, 100, 1, true, {26, 13, 0, 100, 2.25f, 0.75f, 0.2f});
    auto path = combined_algorithm(matrix, edgeCount, read_previous_result(matrix));
    cout << "Genetics elapsed time: [" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - geneticsAlgoBeginTime).count() << "] min." << endl;

    vector<string> resultCities;

    try {
        cout << "CONGRATULATION!!! Path length: [" << validate_city_list(resultCities, path) << "] symbols." << endl;
    } catch(char const*ex) { cout << ex << endl; }

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
            throw "Erorr!!! Path isn't valid!!!";
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
