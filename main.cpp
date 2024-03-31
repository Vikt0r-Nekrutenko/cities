#include <fstream>
#include <iterator>
#include <chrono>

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
    // vector<pair<Genome, size_t>> genomes(8);
    /*
Regular:.....[7]
Elite:.......[6]
Iterations:..[3]
Alpha:.......[2.44162]
Beta:........[1.42196]
Evaporation:.[0.163356]
Length:......[16504]
*/
    // for(auto &genome : genomes) {
    //     genome.first.regularAntCount    = randd(1, 26);
    //     genome.first.eliteAntCount      = randd(1, 5);
    //     genome.first.iterations         = randd(1, 30);
    //     genome.first.evaporation        = randf(0.1f, 1.f);
    //     genome.first.alpha              = randf(0.1f, 4.f);
    //     genome.first.beta               = randf(0.1f, 4.f);
    // }
    // Genome bestGenome {3, 3, 3, 1.f, 1.f, 0.5f};
    // pair<Path, size_t> bestResult;
    // size_t totalBestLength = 0;

    // ifstream mtxFile("matrixes/16504.txt");
    // for(auto &row : matrix)
    //     for(auto &vertex : row)
    //         for(auto &edge : vertex)
    //             mtxFile >> edge.pheromone;
    // mtxFile.close();

    // int rd[] = {-1, 1};
    // float rf[] = {-0.1f, 0.1f};

    // int gen = 0;
    // srand(1998);
    // while(gen < 50) {
        Matrix matrix = Matrix(MATRIX_SIZE, Row(MATRIX_SIZE));
        for(auto &city : available_cities) {
            if(matrix.at(city.front() - 'A').at(city.back() - 'a').empty())
                matrix.at(city.front() - 'A').at(city.back() - 'a').push_back({&city});
        }
        // int randGen = rand() % 6;
        // int colonyN = 0;
        // int randColonyN = rand() % 8;
        // size_t maxGenomeResult = 0;

        // for(auto &genome : genomes) {
            // auto combinedAlgoBeginTime = chrono::high_resolution_clock::now();
            // auto result = combined_algorithm(matrix, genome.first);
            // cout << gen << ":" << colonyN << ".Colony result: [" << result.second << "] elapsed: " << chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - combinedAlgoBeginTime).count() << "sec." << endl;
            // genome.second = result.second;
            // if(genome.second > maxGenomeResult) {
            //     maxGenomeResult = genome.second;
            //     bestGenome = genome.first;
            //     bestResult.second = genome.second;
            // }
            // if(genome.second > totalBestLength) {
                // totalBestLength = genome.second;
                // ofstream mtxFile("matrixes/" + to_string(totalBestLength) + ".txt");
                // for(const auto &row : matrix)
                //     for(const auto &vertex : row)
                //         for(const auto &edge : vertex)
                //             mtxFile << edge.pheromone << " ";
                // mtxFile.close();
            // }
            // genome.first = bestGenome;
            // if(colonyN == randColonyN)
            // {
            //     switch (randGen) {
            //     case 0: genome.first.regularAntCount = randd(1, 27); break;
            //     case 1: genome.first.eliteAntCount = randd(1, 6); break;
            //     case 2: genome.first.iterations = randd(1, 31); break;
            //     case 3: genome.first.evaporation = randf(0.1f, 1.f); break;
            //     case 4: genome.first.alpha = randf(0.1f, 4.f); break;
            //     case 5: genome.first.beta = randf(0.1f, 4.f); break;
            //     }
                // switch (randGen) {
                // case 0: {
                //     int newRegularAntCount = genome.first.regularAntCount + rd[rand() % 2];
                //     if(newRegularAntCount > 1 && newRegularAntCount < 27)
                //         genome.first.regularAntCount = newRegularAntCount;
                //     break;
                // }
                // case 1: {
                //     int newEliteAntCount = genome.first.eliteAntCount + rd[rand() % 2];
                //     if(newEliteAntCount > 1 && newEliteAntCount < 27)
                //         genome.first.eliteAntCount = newEliteAntCount;
                //     break;
                // }
                // case 2: {
                //     int newIterations = genome.first.iterations + rd[rand() % 2];
                //     if(newIterations > 1 && newIterations < 6)
                //         genome.first.iterations = newIterations;
                //     break;
                // }
                // case 3: {
                //     float newEvaporation = genome.first.evaporation + rf[rand() % 2];
                //     if(newEvaporation > 0.1 && newEvaporation < 1.f)
                //         genome.first.evaporation = newEvaporation;
                //     break;
                // }
                // case 4: {
                //     float newAlpha = genome.first.alpha + rd[rand() % 2];
                //     if(newAlpha > 0.1 && newAlpha < 5.f)
                //         genome.first.alpha = newAlpha;
                //     break;
                // }
                // case 5: {
                //     float newBeta = genome.first.beta + rd[rand() % 2];
                //     if(newBeta > 0.1 && newBeta < 5.f)
                //         genome.first.beta = newBeta;
                //     break;
                // }
                // }
            // }
            // ++colonyN;
        // }
        // cout << "Generation:..[" << gen << "]" << endl
             // << "Regular:.....[" << bestGenome.regularAntCount << "]" << endl
             // << "Elite:.......[" << bestGenome.eliteAntCount << "]" << endl
             // << "Iterations:..[" << bestGenome.iterations << "]" << endl
             // << "Alpha:.......[" << bestGenome.alpha << "]" << endl
             // << "Beta:........[" << bestGenome.beta << "]" << endl
             // << "Evaporation:.[" << bestGenome.evaporation << "]" << endl
             // << "Length:......[" << bestResult.second << "]" << endl;
        // ++gen;
    // }
    // cout << "CONGRATULATION!!! Path length: [" << totalBestLength << "] symbols." << endl;
    // return available_cities;

    auto geneticsAlgoBeginTime = chrono::high_resolution_clock::now();
    // auto path = combined_algorithm(matrix, {25, 13, 10, 1.f, 4.f, 0.65f}); // avg time: 476s; path lenth: 16173 symbols. 55s:16112 FOR 10 ITERATIONS!!!!
    auto path = genetics_algorithm(matrix, 10, 8);
    cout << "Genetics elapsed time: [" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - geneticsAlgoBeginTime).count() << "] millisec." << endl;
    // cout << "Combined elapsed time: [" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - combinedAlgoBeginTime).count() << "] millisec." << endl;

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
