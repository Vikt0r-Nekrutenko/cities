#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <array>
#include <chrono>
#include <algorithm>

using namespace std;

const uint8_t MATRIX_SIZE = 26;

typedef struct _edge_t{
    string value;
    float  tao;
} edge_t;

class vertex : public vector<edge_t> {
public:
    void print(){
        for_each(begin(), end(), [](edge_t &edge){
            cout << edge.value << " " << edge.value.size() << " " << edge.tao << endl;
        });
    }

    void sort() {
        for(vertex::iterator it = vector<edge_t>::begin(); it != end() - 1; it++)
            for(vertex::iterator sub_it = it + 1; sub_it != end(); sub_it++)
                if(it->value.length() > sub_it->value.length())
                    ::swap(*it, *sub_it);
    }

    void rm_passed_edge(){
        pop_back();
        mend = size() - 1;
    }
    void add_edge(edge_t edge){
        push_back(edge);
        mend = size() - 1;
    }

    float all_tao(){
        float total_tao = 0;
        for_each(begin(), end(), [&](edge_t &edge){
            total_tao += edge.tao;
        });
        return total_tao;
    }

    size_t mend;
};

class row : private array<vertex, MATRIX_SIZE> {
public:
    using array<vertex, MATRIX_SIZE>::operator[];
    void print(bool show_tao = false){
        if(!show_tao)
            for_each(begin(), end(), [](vertex &vrtx){
                cout << vrtx.size() << (vrtx.size() > 9 ? " " : "  ");
            });
        else
            for_each(begin(), end(), [](vertex &vrtx){
                cout << vrtx.all_tao() << (vrtx.all_tao() > 9 ? " " : "  ");
            });
    }

    void sort(){
        for_each(begin(), end(), [](vertex &vrtx){
            if(!vrtx.empty())
                vrtx.sort();
        });
    }

    vector<vertex*> row_except_empty_vertex(){
        vector<vertex*> new_row;
        for(row::iterator it = begin(); it != end(); it++)
            if(!it->empty()/* && !it->front().is_passed*/)
                new_row.push_back(it);
        return new_row;
    }

    vertex *get_random_vertex(){
        vector<vertex*> new_row = row_except_empty_vertex();
        if(new_row.empty()) return nullptr;

        edge_t *random_edge = nullptr;

        for(size_t i = rand() % new_row.size(); i < new_row.size(); i++){
            random_edge = &new_row[i]->back();
            if(random_edge != nullptr)
                return &at(random_edge->value.back() - 97);
        }
        return nullptr;
    }

    vertex *get_vertex_with_max_tao(){
        vector<vertex*> new_row = row_except_empty_vertex();
        if(new_row.empty()) return nullptr;

        vertex *max_tao = new_row[0];

        for(size_t i = 0; i < new_row.size(); i++){
            if(max_tao->at(max_tao->mend).tao < new_row[i]->at(new_row[i]->mend).tao)
                max_tao = new_row[i];
        }
        return max_tao;
    }
};

class ant;

class graph : private array<row, MATRIX_SIZE> {
public:
    using array<row, MATRIX_SIZE>::operator[];
    graph(vector<string> source_list) : array<row, MATRIX_SIZE>() {
        for_each(source_list.begin(), source_list.end(), [&](string word){
            if(word.front() - 65 != word.back() - 97)
                (*this)[word.front() - 65][word.back() - 97].add_edge({word, 0});
        });
    }
    void print(){
        for_each(begin(), end(), [](row &r){
            r.print(true);
            cout <<  endl;
        });
    }
    void sort(){
        for_each(begin(), end(), [](row &row_ref){
            row_ref.sort();
        });
    }

    void evaporation(ant &a);
};

class ant{
public:
    ant(graph &map) : personal_map(map), global_map(&map) {}
    void move_to_next_vertex(uint8_t begin_vertex){
        vertex *current_vrtx = personal_map[begin_vertex - 65].get_random_vertex();
        do{
            path.push_back(current_vrtx->back().value);
            (*global_map)[current_vrtx->back().value.front() - 65][current_vrtx->back().value.back() - 97][current_vrtx->mend].tao+=3;
            current_vrtx->rm_passed_edge();
            current_vrtx = personal_map[path.back().back() - 97].get_random_vertex();
        }while(current_vrtx != nullptr);
    }
    void move_to_next_vertex(){
        vertex *current_vrtx = personal_map['E' - 65].get_vertex_with_max_tao();
        do{
            path.push_back(current_vrtx->back().value);
            (*global_map)[current_vrtx->back().value.front() - 65][current_vrtx->back().value.back() - 97][current_vrtx->mend].tao++;
            current_vrtx->rm_passed_edge();
            if(rand() % 1)
                current_vrtx = personal_map[path.back().back() - 97].get_vertex_with_max_tao();
            else
                current_vrtx = personal_map[path.back().back() - 97].get_random_vertex();
        }while(current_vrtx != nullptr);
    }

    vector<string> path;
    graph          personal_map;
    graph          *global_map;
};

void graph::evaporation(ant &a){
    size_t l = 0;
    for(auto i : a.path)
        l += i.length();

    for(uint8_t i = 0; i < MATRIX_SIZE; i++)
        for(uint8_t j = 0; j < MATRIX_SIZE; j++)
            for(size_t x = 0; x < (*this)[j][i].size(); x++)
                if((*this)[j][i][x].tao > 0)
                    (*this)[j][i][x].tao = (1 - 0.2f)*(*this)[j][i][x].tao + 1/l;
    for(auto i : a.path){
        (*this)[i.front() - 65][i.back() - 97][]
    }
}

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
int write_to_file(vector<string> cities_list);

int main() 
{
  vector<string> available_cities = read_available_cities();

  vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

  return write_to_file(cities_list);
}

vector<string> combine_cities(vector<string> available_cities) {
    graph g1(available_cities);
    g1.sort();

//    srand(time(0));

    ant a1(g1);
    a1.move_to_next_vertex('E');
    g1.evaporation(a1);

//    int iter = 0;
//    while(iter++ < 500){
//        a1 = ant(g1);
//        a1.move_to_next_vertex();
//        g1.evaporation();
////        cout << iter << " " << a1.path.size() << endl;
//    }

//    a1 = ant(g1);
//    a1.move_to_next_vertex();
//    cout << a1.path.size() << endl;

    g1.print();
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

