#include <fstream>
#include <iterator>
#include <chrono>
#include <iostream>

// #include "genetic_algorithm.hpp"
#include "combined_algorithm.hpp"

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
size_t validate_city_list(vector<string> &resultCities, const pair<Path, size_t> &path);

int write_to_file(vector<string> cities_list);

int main()
{
    vector<string> available_cities = read_available_cities();

    vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

    return write_to_file(cities_list);
}
#include <cmath>
vector<string> combine_cities(vector<string> available_cities)
{
    srand(1998);
    // srand(time(nullptr));

    Matrix3d mtx;// = Matrix(MATRIX_SIZE, vector<vector<Edge>>(MATRIX_SIZE));
    vector<vector<Edge>> matrix{MATRIX_SIZE};
    for(auto &city : available_cities) {
        if(mtx[city.front() - 'A'][city.back() - 'a'].empty()) {
        // if(matrix[city.front() - 'A'].edges[city.back() - 'a'].word == nullptr)
            mtx[city.front() - 'A'][city.back() - 'a'].push_back({&city});
            matrix[city.front() - 'A'].push_back({&city});
        }
    }
    // auto time1 = chrono::high_resolution_clock::now();
    // cout << "time1: [" << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - time1).count() << "] min." << endl;
    // return available_cities;
        // tmpMatrix[city.front() - 'A'][city.back() - 'a'].push_back({&city});
    // ifstream mtxFile("matrixes/16579.txt");
    // for(int x = MATRIX_SIZE - 1; x >= 0; --x)
    //     for(int y = MATRIX_SIZE - 1; y >= 0; --y)
    //         for(int z = matrix[x][y].size() - 1; z >= 0; --z)
    //             mtxFile >> matrix[x][y][z].pheromone;
    // mtxFile.close();

    auto geneticsAlgoBeginTime = chrono::high_resolution_clock::now();
    // auto path = genetics_algorithm(matrix, 3, 4, true, {26, 1, 1, 0.742006f, 0.118091f, 0.581106f});
    auto path = combined_algorithm(mtx, {26, 52, 1, 1000, 2.f, 1.f, 0.1f});
    cout << "Genetics elapsed time: [" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - geneticsAlgoBeginTime).count() << "] min." << endl;

    vector<string> resultCities;

    try {
        cout << "CONGRATULATION!!! Path length: [" << validate_city_list(resultCities, path) << "] symbols." << endl;
    } catch(const string &ex) { cout << ex << endl; }

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

size_t validate_city_list(vector<string> &resultCities, const pair<Path, size_t> &path)
{
    size_t length = 0ull;

    int validationSymbol = path.first.front()->word->front() - 'A';
    for(const auto &edge : path.first) {
        if(edge->word->front() - 'A' != validationSymbol) {
            throw "Erorr!!! Path isn't valid!!!";
        }
        validationSymbol = edge->word->back() - 'a';
        length += edge->word->length();
        resultCities.push_back(*edge->word);
    }
    return length;
}
