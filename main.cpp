#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stack>
#include <chrono>

using namespace std;

typedef struct _vertex_t{
    uint16_t value;
    uint16_t edges_to_end;
} vertex_t;

typedef struct _edge_t{
    uint16_t value;
    uint16_t vrtx_to_end;
} edge_t;

typedef vector<vector<vertex_t>> adjacency_matrix_t;
typedef vector<vector<edge_t>>   incidence_matrix_t;

typedef struct _frame_t{
    adjacency_matrix_t &matrix;
    uint8_t target;
} frame_t;

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
int write_to_file(vector<string> cities_list);
adjacency_matrix_t build_adjacency_matrix(vector<string> &cities);
incidence_matrix_t build_incidence_matrix();
void print_matrix(adjacency_matrix_t &matrix);
void print_matrix(incidence_matrix_t &matrix);
string dfs(adjacency_matrix_t &mtrx, incidence_matrix_t &inc_mtrx, uint8_t target);
string find_existing_edges(incidence_matrix_t &inc_mtrx, string &result);
uint8_t find_max_edges_to_end(incidence_matrix_t &inc_mtrx, uint8_t row);

int main() 
{
  vector<string> available_cities = read_available_cities();

  vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

  return write_to_file(cities_list);
}

void foo(vector<string> &available_cities, string &str){
    uint32_t n = 0;
    uint32_t l = 0;

    while(n < str.size()){
        for(vector<string>::iterator it = available_cities.begin(); it != available_cities.end(); it++){
            if(it->front() == str[n] && it->back() == str[n+1]){
//                strs.push_back(*it);
                l += it->length();
                it = available_cities.erase(it);
            }
        }
        n += 2;
    }
}

void sort_row(incidence_matrix_t &mtrx, uint8_t index){
    for(uint8_t y = 0; y < 26; y++)
        for(uint8_t x = 0; x < 26; x++)
            if(mtrx[index][y].value > mtrx[index][x].value)
                swap(mtrx[index][y], mtrx[index][x]);
}

void sort_row(adjacency_matrix_t &mtrx, uint8_t index){
    for(uint8_t y = 0; y < 26; y++)
        for(uint8_t x = 0; x < 26; x++)
            if(mtrx[index][y].edges_to_end > mtrx[index][x].edges_to_end)
                swap(mtrx[index][y], mtrx[index][x]);
}

void full_sort(incidence_matrix_t &mtrx){
    for(uint8_t index = 0; index < 26; index++)
        sort_row(mtrx, index);
}

vector<string> combine_cities(vector<string> available_cities) {
  // TODO replace with your solution!

    size_t size;
    cin >> size;
    string str;
    vector<string> strs;
    adjacency_matrix_t matrix;
    incidence_matrix_t inc_mtrx = build_incidence_matrix();

    if(size == 2){
        matrix = build_adjacency_matrix(available_cities);
        str = dfs(matrix, inc_mtrx, 'e' - 97);
        cout << str << " " << str.size() << endl;
//        foo(available_cities, str);

//        matrix = build_adjacency_matrix(available_cities);
//        str += "...SPLEAT HERE!!!..." + dfs(matrix, inc_mtrx, 'g' - 97);
////        cout << str << " " << str.size() << endl;
//        foo(available_cities, str);

//        matrix = build_adjacency_matrix(available_cities);
//        str += "...SPLEAT HERE!!!..." + dfs(matrix, inc_mtrx, 'b' - 97);
////        cout << str << " " << str.size() << endl;
//        foo(available_cities, str);

//        matrix = build_adjacency_matrix(available_cities);
//        str += "...SPLEAT HERE!!!..." + dfs(matrix, inc_mtrx, 'p' - 97);
//        cout << str << " " << str.size() << endl;
//        foo(available_cities, str);

//        matrix = build_adjacency_matrix(available_cities);
//        print_matrix(matrix);

//        cout << available_cities.size() << endl;
    }
    else{
        vector<string> test_list(size == 1 ? 25 : size);
        for(string &i : test_list) i.append({char('A'+rand()%('Z'-'A'+1)), char('a'+rand()%('z'-'a'+1))});
        for(size_t i = test_list.size() - 1; i > 0; i--)
            for(int j = i - 1; j >= 0; j--)
                if(test_list[i].front() < test_list[j].front())
                    swap(test_list[i], test_list[j]);
//        for(string &i : test_list) cout << i << endl;
        matrix = build_adjacency_matrix(test_list);
        str = dfs(matrix, inc_mtrx, 'v' - 97);
        cout << str << " " << str.length() << endl;
    }

//    cout << available_cities.size() << endl;

    return strs;
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

string dfs(adjacency_matrix_t &mtrx, incidence_matrix_t &inc_mtrx, uint8_t target){
    stack<frame_t> stk;
    string result, comp;
    stk.push({mtrx, target});

    do{
        for(uint8_t next = 0; next < 26; next++){
            if(stk.top().matrix[stk.top().target][next].value){
                result.append({char(stk.top().target + 65), char(next + 97)});
                stk.top().matrix[stk.top().target][next].value--;
                inc_mtrx[stk.top().target][next].value++;
                stk.push({stk.top().matrix, next});
                next = 0;
            }
        }

        if(result.size()){
            string str = find_existing_edges(inc_mtrx, result);
//            cout << result << "..." << str << " " << result.length() + str.length() << endl;
            if(result.length() + str.length() > comp.length()) comp = result + str;
            result.pop_back();
            result.pop_back();
        }
        stk.pop();

    }while(!stk.empty());

    return comp;
}

uint8_t find_max_edges_to_end(incidence_matrix_t &inc_mtrx, uint8_t row){
    uint8_t max = 0;
    for(uint8_t i = 0; i < 26; i++){
        if(inc_mtrx[row][i].vrtx_to_end >= inc_mtrx[row][max].vrtx_to_end && inc_mtrx[row][i].value){
            max = i;
        }
    }
    return max;
}

string find_existing_edges(incidence_matrix_t &inc_mtrx, string &result){
    incidence_matrix_t tmp_inc_mtrx = inc_mtrx;
    string existing_edges;
    uint8_t target = result.back() - 97;

    for(size_t i = 0; i < result.size(); i += 2)
        tmp_inc_mtrx[result[i] - 65][result[i + 1] - 97].value--;

    uint8_t f = *(result.end() - 2) - 65;
    uint8_t l = *(result.end() - 1) - 97;

    for(uint8_t s = 0; s < 26; s++){
        if(tmp_inc_mtrx[target][s].value){
            uint8_t ss = find_max_edges_to_end(tmp_inc_mtrx, target);
            if(tmp_inc_mtrx[target][ss].vrtx_to_end > tmp_inc_mtrx[target][s].vrtx_to_end && tmp_inc_mtrx[target][ss].value)
                s = ss;
            existing_edges.append({ char(target + 65), char(s + 97) });
            tmp_inc_mtrx[target][s].value--;
            inc_mtrx[f][l].vrtx_to_end++;
//            cout << char(f+65) << char(l+97) << " ";
            target = s;
            s = 0;
        }
    }
//    cout << endl;
    return existing_edges;
}

int write_to_file(vector<string> cities_list) {
  ofstream output_file("./output.txt");
  ostream_iterator<string> output_iterator(output_file, "\n");
  copy(cities_list.begin(), cities_list.end(), output_iterator);

  return 0;
} 

adjacency_matrix_t build_adjacency_matrix(vector<string> &source){
    adjacency_matrix_t matrix(26);

    uint16_t index = 0;
    do{
        matrix[index] = vector<vertex_t>(26);
    }while(++index < 26);

    for(vector<string>::iterator word = source.begin(); word != source.end(); word++){
        matrix[word->front() - 65][word->back() - 97].value++;
    }
    return matrix;
}

incidence_matrix_t build_incidence_matrix(){
    incidence_matrix_t matrix(26);

    uint16_t index = 0;
    do{
        matrix[index] = vector<edge_t>(26);
    }while(++index < 26);

    return matrix;
}

void print_matrix(adjacency_matrix_t &mtrx){
    for(uint8_t i = 'A'; i <= 'Z'; i++){
        for(uint8_t j = 'a'; j <= 'z'; j++){
            cout << i << j << " " << mtrx[i-65][j-97].value << ":" << mtrx[i-65][j-97].edges_to_end << " ";
        }
        cout << endl;
    }
}

void print_matrix(incidence_matrix_t &mtrx){
    for(uint8_t i = 'A'; i <= 'Z'; i++){
        for(uint8_t j = 'a'; j <= 'z'; j++){
            cout << i << j << " " << mtrx[i-65][j-97].value << ":" << mtrx[i-65][j-97].vrtx_to_end << " ";
        }
        cout << endl;
    }
}

