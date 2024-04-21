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

vector<string> combine_cities(vector<string> available_cities)
{
    srand(1998);
    // srand(time(nullptr));

    Matrix3d mtx;// = Matrix(MATRIX_SIZE, vector<vector<Edge>>(MATRIX_SIZE));
    Matrix2d matrix;
    for(auto &city : available_cities) {
        if(mtx[city.front() - 'A'][city.back() - 'a'].empty()) {
        // if(matrix[city.front() - 'A'].edges[city.back() - 'a'].word == nullptr)
            mtx[city.front() - 'A'][city.back() - 'a'].push_back({&city});
            matrix[city.front() - 'A'].first = true;
            matrix[city.front() - 'A'].second.push_back({&city});
        }
    }
    auto time1 = chrono::high_resolution_clock::now();
    struct PaTh {
        Edge *edges[1000];
        Edge **end;
    };

    pair<PaTh, size_t> colonyBestPathPairs[1];
    pair<PaTh, size_t> *colonyPathPairPtr = colonyBestPathPairs;
    int vertexNumber = 0;

    pair<PaTh, size_t> workingStack {{}, 0};
    Edge **it = workingStack.first.edges;

    while(true) {
        bool isEnd = true;
        Edge *selectedEdge = nullptr;
        Edge *ptr = matrix[vertexNumber].second.data();
        Edge *end = matrix[vertexNumber].second.data() + matrix[vertexNumber].second.size();
        while(ptr != end) {
            Edge &edge = *ptr++;
            if(edge.isPassed)
                continue;
            selectedEdge = &edge;
            isEnd = false;
            break;
        }

        if(isEnd) {
            // if(antPathPairs.back().first.size() <= 1) {
            //     *colonyPathPairPtr = antPathPairs[bestPathForOneAntIndex];
            //     break;
            // }
            // if(antPathPairs.back().second > bestPathForOneAntLength) {
            //     bestPathForOneAntIndex = antPathPairs.size() - 1;
            //     bestPathForOneAntLength = antPathPairs.back().second;
            // }

            // auto lastEdge = antPathPairs.back().first.back();
            // auto prevEdge = antPathPairs.back().first.end() - 2;
            // antPathPairs.push_back({{antPathPairs.back().first.begin(), prevEdge},
            //                         antPathPairs.back().second - lastEdge->word->length()});

            // if(workingStack.first.size() <= 1)
            //     break;
            // if(workingStack.second > colonyPathPairPtr->second)
            //     *colonyPathPairPtr = workingStack;
            // workingStack.second -= workingStack.first.back()->word->length();
            // vertexNumber = workingStack.first.back()->word->front() - 'A';
            // workingStack.first.pop_back();

            if(it == workingStack.first.edges + 1)
                break;
            if(workingStack.second > colonyPathPairPtr->second) {
                colonyPathPairPtr->second = workingStack.second;
                Edge **tmpPtrA = workingStack.first.edges;
                Edge **tmpPtrB = colonyPathPairPtr->first.edges;
                while(tmpPtrA != it) {
                    *tmpPtrB = *tmpPtrA;
                    ++tmpPtrA;
                    ++tmpPtrB;
                }
                colonyPathPairPtr->first.end = tmpPtrB;
                // *colonyPathPairPtr = {{workingStack.first.begin(), it}, workingStack.second};
            }
            --it;
            workingStack.second -= (*it)->word->length();
            vertexNumber = (*it)->word->front() - 'A';
            continue;
        }

        vertexNumber = selectedEdge->word->back() - 'a';
        selectedEdge->isPassed = true;

        // workingStack.first.push_back(selectedEdge);
        *it++ = selectedEdge;
        workingStack.second += selectedEdge->word->length();

        // antPathPairs.back().first.push_back(selectedEdge);
        // antPathPairs.back().second += selectedEdge->word->length();
    }
    cout << "time1: [" << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - time1).count() << "] min." << endl;
    cout << colonyBestPathPairs[0].second << endl;
    int l = 0;
    int validationSymbol = colonyBestPathPairs[0].first.edges[0]->word->front() - 'A';
    Edge **i = colonyBestPathPairs[0].first.edges;
    while(i != colonyBestPathPairs[0].first.end) {
        if((*i)->word->front() - 'A' != validationSymbol) {
            throw "Erorr!!! Path isn't valid!!!";
        }
        validationSymbol = (*i)->word->back() - 'a';
        l += (*i)->word->length();
        ++i;
    }
    cout << l << endl;
    return available_cities;
        // tmpMatrix[city.front() - 'A'][city.back() - 'a'].push_back({&city});
    // 17:39 - 19:53 [2:13] = 16595/10000
    // 20:51 - 21:41 [1:00] = 16564/1500?
    // 1800
    // ifstream mtxFile("matrixes/16680.txt");
    // for(int x = MATRIX_SIZE - 1; x >= 0; --x) {
    //     Edge *ptr = matrix[x].second.data();
    //     Edge *end = matrix[x].second.data() + matrix[x].second.size();
    //     while(ptr != end) {
    //         mtxFile >> ptr->pheromone;
    //         ++ptr;
    //     }}
    // mtxFile.close();

    // auto geneticsAlgoBeginTime = chrono::high_resolution_clock::now();
    // // auto path = genetics_algorithm(matrix, 100, 1, true, {26, 13, 0, 100, 2.25f, 0.75f, 0.2f});
    // auto path = combined_algorithm(matrix, {26, 13, 1, 1'000, 2.25f, 0.75f, 0.3f});
    // cout << "Genetics elapsed time: [" << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - geneticsAlgoBeginTime).count() << "] min." << endl;

    vector<string> resultCities;

    try {
        // cout << "CONGRATULATION!!! Path length: [" << validate_city_list(resultCities, colonyBestPathPairs[0]) << "] symbols." << endl;
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
