#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <ctime>

#include "matrix.hpp"
#include "ant.hpp"
#include "explorer.hpp"
#include "elite.hpp"
#include "roulette.hpp"

using namespace std;

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
int write_to_file(vector<string> cities_list);

class deep_explorer : public ant {
public:
    deep_explorer(matrix *map) : ant(map, 25) {}
    void step_back(){
        m_path.rm_passed_edge();
        m_location = choose_next_vertex();
//        personal_map[em.word.front() - 65][em.word.back() - 97].push_back(em);
    }

    void step_forward() override {
        path tm_p;
        while(can_move())
        {
            ant::step_forward();

            if(m_path.get_pl() > 15000.f)
//            if(tm_p.get_pl() < m_path.get_pl()){
                tm_p = m_path;
//            }

//            if(m_location == nullptr)
            while(m_location == nullptr && m_path.size() > 1)
                step_back();
        }
        m_path = tm_p;
    }
};

int main() 
{
  vector<string> available_cities = read_available_cities();

  vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

  return write_to_file(cities_list);
}

void move_exp(matrix &mtrx, int steps){
    explorer ex(&mtrx, 4);
    int iter = 0;
    while(iter++ <= steps)
    {
        while(ex.can_move())
            ex.step_forward();
        ex.update_pheromone();
        ex = explorer(&mtrx, 4);
    }
}

void move_elite(matrix &mtrx, int steps){
    elite e(&mtrx, 4);
    int iter = 0;
    while(iter++ <= steps)
    {
        while(e.can_move())
        {
            e.step_forward();
        }
        e.update_pheromone();
        e = elite(&mtrx, 4);
    }
}

void move_ant(matrix &mtrx, int steps){
    ant a(&mtrx, 4);
    int iter = 0;
    while(iter++ <= steps)
    {
        while(a.can_move())
            a.step_forward();
        a.update_pheromone();

        if(iter % 26 == 0){
            mtrx.evaporation();
//            move_elite(mtrx, 5);
        }
        a = ant(&mtrx, 4);
    }
}

void move_deep(matrix &mtrx, int steps){
    deep_explorer de(&mtrx);
    int iter = 0;
    while(iter++ <= steps)
    {
//            FILE *f = fopen(string(to_string(iter) + ".txt").c_str(), "w");
        de.step_forward();
//            for(auto i : de.get_path()){
//                fwrite(i.word.c_str(), i.word.length(), 1, f);
//                fwrite("\n", 1, 1, f);
//            }
        de.update_pheromone();
        de = deep_explorer(&mtrx);
    }
    cout << endl;
}

vector<string> combine_cities(vector<string> available_cities) {
    matrix mtrx(available_cities);
    mtrx.sort();

    srand(time(nullptr));
    move_deep(mtrx, 200);

    mtrx[25][4].print();

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

