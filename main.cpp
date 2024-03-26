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

    Genome genomes[8];

    for(auto &genome : genomes) {
        Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
        for(auto &city : available_cities) {
            matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
        }

        genome.regularAntCount = randd(10, 26);
        genome.eliteAntCount = randd(10, 26);
        genome.iterations = randd(1, 6);
        genome.evaporation = randf(0.f, 1.f);
        genome.alpha = randf(0.f, 4.f);
        genome.beta = randf(0.f, 4.f);

        combined_algorithm(matrix, genome);
        cout << "NEXT GENOME" << endl;
    }

    // auto combinedAlgoBeginTime = chrono::high_resolution_clock::now();
    // Path path = combined_algorithm(matrix, {25, 13, 10, 1.f, 4.f, 0.65f}); // avg time: 476s; path lenth: 16173 symbols. 55s:16112 FOR 10 ITERATIONS!!!!
    // cout << "Combined elapsed time: [" << chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - combinedAlgoBeginTime).count() << "] sec." << endl;

    // size_t length = 0ull;
    // vector<string> resultCities;

    // int validationSymbol = path.front()->word->front() - 'A';
    // for(const auto &edge : path) {
    //     if(edge->word->front() - 'A' != validationSymbol) {
    //         cout << "Erorr!!! Path isn't valid!!!" << endl;
    //         throw;
    //     }
    //     validationSymbol = edge->word->back() - 'a';
    //     length += edge->word->length();
    //     resultCities.push_back(*edge->word);
    // }
    // cout << "CONGRATULATION!!! Path length: [" << length << "] symbols." << endl;

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
