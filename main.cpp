#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stack>
#include <chrono>
#include <windows.h>

HANDLE Out;

using namespace std;

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
int write_to_file(vector<string> cities_list);

typedef struct _vertex_t{ uint16_t value; }vertex_t;

typedef vector<vertex_t> row_t;
typedef vector<row_t> adjacency_matrix_t;
typedef vector<row_t> incidence_matrix_t;

typedef struct _frame_t{
    adjacency_matrix_t &matrix;
    uint8_t first;
    uint8_t last;
} frame_t;

adjacency_matrix_t build_adjacency_matrix(vector<string> &cities);
incidence_matrix_t build_incidence_matrix();

void print_matrix(adjacency_matrix_t &matrix);
void dfs(adjacency_matrix_t &matrix, uint8_t first, uint8_t next);

int main() 
{
  vector<string> available_cities = read_available_cities();

  vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

  return write_to_file(cities_list);
}

vector<string> combine_cities(vector<string> available_cities) {
  // TODO replace with your solution!
    Out = GetStdHandle(STD_OUTPUT_HANDLE);

    size_t size;
    cin >> size;
    vector<string> test_list(size == 1 ? 25 : size);
    for(string &i : test_list) i.append({char('A'+rand()%('Z'-'A'+1)), char('a'+rand()%('z'-'a'+1))});
    for(size_t i = test_list.size() - 1; i > 0; i--)
        for(int j = i - 1; j >= 0; j--)
            if(test_list[i].front() < test_list[j].front())
                swap(test_list[i], test_list[j]);
//    for(string &i : test_list) cout << i << endl;

    adjacency_matrix_t matrix = build_adjacency_matrix(test_list);

    auto ts = chrono::high_resolution_clock().now();
    dfs(matrix, 'z'-97, 'v'-97);
    auto te = chrono::high_resolution_clock().now();
    cout << chrono::duration<double, milli>(te-ts).count() / 1000 << " secs" << endl;
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

string find_existing_edges(incidence_matrix_t &inc_mtrx, uint8_t target){
    uint8_t _next = target;
    string existing_edges;

    for(char s = 0; s < 26; s++){
        if(inc_mtrx[_next][s].value){
            existing_edges.append({ char(_next + 65), char(s + 97) });
            inc_mtrx[_next][s].value--;
            _next = s;
            s = 0;
        }
    }
    return existing_edges;
}

void dfs(adjacency_matrix_t &matrix, uint8_t first, uint8_t next){
    stack<frame_t> stk;
    incidence_matrix_t inc_mtrx = build_incidence_matrix();
    stk.push({matrix, first, next});
    string result;


    uint16_t counter = 0;

    while(!stk.empty())
    {
        uint8_t previous = 0;
        uint8_t current  = 0;

        for(uint8_t next = 0; next < 26; next++)
        {
            previous = stk.top().first;
            current = stk.top().last;
            adjacency_matrix_t &mtrx_ref = stk.top().matrix;

            if(inc_mtrx[current][next].value){
                cout << ++counter << " " << result << "..." << char(current+65) << char(next+97) << "<-FINDED...";

                incidence_matrix_t tmp_inc_mtrx = inc_mtrx;
                string target = find_existing_edges(tmp_inc_mtrx, next);
                cout << target << " " << target.length() + result.length() << endl;

//                current = *(target.end()-2) - 97;
//                next = *(target.end()-1) - 65;
            }
            else if(mtrx_ref[current][next].value)
            {
//                cout << "PUSH" << endl;
                mtrx_ref[current][next].value--;
                stk.push({mtrx_ref, current, next});

                inc_mtrx[current][next].value++;
                result.append({char(current+65), char(next+97)});

                next = 0;
            }
        }

        if(result.size()){
           result.pop_back();
           result.pop_back();
        }
        stk.pop();
    }
//    print_matrix(inc_mtrx);
}

int write_to_file(vector<string> cities_list) {
  ofstream output_file("./output.txt");
  ostream_iterator<string> output_iterator(output_file, "\n");
  copy(cities_list.begin(), cities_list.end(), output_iterator);

  return 0;
} 

adjacency_matrix_t build_adjacency_matrix(vector<string> &cities){
    adjacency_matrix_t matrix(26);

    uint16_t index = 0;
    do{
        matrix[index] = row_t(26);
    }while(++index < 26);

    for(vector<string>::iterator word = cities.begin(); word != cities.end(); word++){
        matrix[size_t(word->front() - 65)][size_t(word->back() - 97)].value++;
    }
    return matrix;
}

incidence_matrix_t build_incidence_matrix(){
    incidence_matrix_t matrix(26);

    uint16_t index = 0;
    do{
        matrix[index] = row_t(26);
    }while(++index < 26);

    return matrix;
}

void print_matrix(adjacency_matrix_t &matrix){
    for(uint8_t i = 'A'; i <= 'Z'; i++){
        for(uint8_t j = 'a'; j <= 'z'; j++){
            cout << i << j << matrix[i-65][j-97].value << (matrix[i-65][j-97].value > 9 ? " " : "  ");
        }
        cout << endl;
    }
}
