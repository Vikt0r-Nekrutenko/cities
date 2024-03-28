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
    vector<pair<Genome, size_t>> genomes(8);
    Genome bestGenome {26, 13, 5, 1.f, 4.f, 0.65f};
    pair<Path, size_t> bestResult;

    Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
    for(auto &city : available_cities) {
        matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
    }

    int gen = 0;
    while(gen < 100) {
        int randGen = rand() % 6;
        int colonyN = 0;
        int randColonyN = rand() % 8;

        for(auto &genome : genomes) {

            genome.first.regularAntCount  = bestGenome.regularAntCount    + (colonyN == randColonyN && randGen == 0 ? randd(-1, 1) : 0);
            genome.first.eliteAntCount    = bestGenome.eliteAntCount      + (colonyN == randColonyN && randGen == 1 ? randd(-1, 1) : 0);
            genome.first.iterations       = bestGenome.iterations         + (colonyN == randColonyN && randGen == 2 ? randd(-1, 1) : 0);
            genome.first.evaporation      = bestGenome.evaporation        + (colonyN == randColonyN && randGen == 3 ? randf(-0.1f, 0.1f) : 0.f);
            genome.first.alpha            = bestGenome.alpha              + (colonyN == randColonyN && randGen == 4 ? randf(-0.1f, 0.1f) : 0.f);
            genome.first.beta             = bestGenome.beta               + (colonyN == randColonyN && randGen == 5 ? randf(-0.1f, 0.1f) : 0.f);

            srand(1998);
            auto combinedAlgoBeginTime = chrono::high_resolution_clock::now();
            auto result = combined_algorithm(matrix, genome.first);
            genome.second = result.second;
            cout << gen << ":" << colonyN++ << ".Colony result: [" << result.second << "] elapsed: " << chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - combinedAlgoBeginTime).count() << "sec." << endl;
        }
        for(auto &genome : genomes)
            if(genome.second > bestResult.second) {
                bestGenome = genome.first;
                bestResult.second = genome.second;
            }
        ++gen;
    }

    cout << "Regular:.....[" << bestGenome.regularAntCount << "]" << endl
         << "Elite:.......[" << bestGenome.eliteAntCount << "]" << endl
         << "Iterations:..[" << bestGenome.iterations << "]" << endl
         << "Alpha:.......[" << bestGenome.alpha << "]" << endl
         << "Beta:........[" << bestGenome.beta << "]" << endl
         << "Evaporation:.[" << bestGenome.evaporation << "]" << endl
         << "Length:......[" << bestResult.second << "]" << endl;

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
