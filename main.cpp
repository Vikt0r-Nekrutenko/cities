#include <fstream>
#include <iterator>
#include <chrono>

#include "combined_algorithm.hpp"

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
    srand(1998);
    Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
    for(auto &city : available_cities) {
        matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
    }

    auto combinedAlgoBeginTime = chrono::high_resolution_clock::now();
    Path path = combined_algorithm(matrix); // avg time: 532s; path lenth: 16081 symbols
    cout << "Combined elapsed time: [" << chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - combinedAlgoBeginTime).count() << "] sec." << endl;
    // auto dfsAlgoBeginTime = chrono::high_resolution_clock::now();
    // Path path = dfs_algorithm(matrix); // avg time: 20s; path lenth: 16043 symbols
    // cout << "DFS elapsed time: [" << chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - dfsAlgoBeginTime).count() << "] sec." << endl;

    // auto antsColonyAlgoBeginTime = chrono::high_resolution_clock::now();
    // Path path = ants_colony_algorithm(matrix); // avg time: 75 path lenth: 10519 symbols
    // cout << "Ant colony elapsed time: [" << chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - antsColonyAlgoBeginTime).count() << "] sec." << endl;

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
