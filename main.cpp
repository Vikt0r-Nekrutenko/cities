#include <fstream>
#include <iterator>

#include "genetic_algorithm.hpp"

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

vector<string> combine_cities(vector<string> available_cities)
{
    srand(1998);

    Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
    for(auto &city : available_cities)
        matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
    pullPheromonesIntoMatrix(matrix, "matrixes/16644.txt");

    auto geneticsAlgoBeginTime = chrono::high_resolution_clock::now();
    auto path = genetics_algorithm(matrix, 3, 4, true, {26, 1, 1, 0.742006f, 0.118091f, 0.581106f});
    cout << "Genetics elapsed time: [" << chrono::duration_cast<chrono::minutes>(chrono::high_resolution_clock::now() - geneticsAlgoBeginTime).count() << "] min." << endl;

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
