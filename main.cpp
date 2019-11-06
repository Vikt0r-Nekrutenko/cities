#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <thread>

#include "matrix.hpp"
#include "explorer.hpp"

using namespace std;

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
int write_to_file(vector<string> cities_list);

int main() 
{
  vector<string> available_cities = read_available_cities();

  vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

  return write_to_file(cities_list);
}
#include <atomic>
vector<string> combine_cities(vector<string> available_cities) {
    matrix mtrx(available_cities);
    mtrx.sort();

    atomic<size_t> max_path(0U);

    explorer a1(mtrx, 4, available_cities.size());
    while(1)
    {
        a1.move();
//        if(a1.passed_path().length() > max_path)
        {
            max_path = a1.passed_path().length();
            cout << max_path << "\t";
        }
        a1.forget_path();
    }

    return vector<string>();
}

vector<string> read_available_cities() {
  string line;
  ifstream input_file ("input.txt");
  vector<string> available_cities;  
  while ( getline (input_file,line) )
  {
    available_cities.push_back(line);
  }
  input_file.close();

  return available_cities;
}

int write_to_file(vector<string> cities_list) {
  ofstream output_file("./output.txt");
  ostream_iterator<string> output_iterator(output_file, "\n");
  copy(cities_list.begin(), cities_list.end(), output_iterator);

  return 0;
} 

