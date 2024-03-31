#include <fstream>
#include <iterator>

#include "genetic_algorithm.hpp"

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
    srand(1433);
    /*
Regular:.....[7]
Elite:.......[6]
Iterations:..[3]
Alpha:.......[2.44162]
Beta:........[1.42196]
Evaporation:.[0.163356]
Length:......[16504]
*/
    Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
    for(auto &city : available_cities) {
        // if(matrix.at(city.front() - 'A').at(city.back() - 'a').empty())
        matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
    }
    pullPheromonesIntoMatrix(matrix, "matrixes/16489.txt");

    auto geneticsAlgoBeginTime = chrono::high_resolution_clock::now();
    auto path = genetics_algorithm(matrix, 10, 8, true);
    cout << "Genetics elapsed time: [" << chrono::duration_cast<chrono::minutes>(chrono::high_resolution_clock::now() - geneticsAlgoBeginTime).count() << "] min." << endl;

    size_t length = 0ull;
    vector<string> resultCities;

    int validationSymbol = path.first.front()->word->front() - 'A';
    for(const auto &edge : path.first) {
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
