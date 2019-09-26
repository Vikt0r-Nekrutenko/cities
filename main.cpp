#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stack>
#include <chrono>

using namespace std;

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
int write_to_file(vector<string> cities_list);

typedef struct _vertex_t{
    string path_to_end;
    uint16_t value;
    char name;
}vertex_t;

typedef vector<vertex_t> row_t;
typedef vector<row_t> adjacency_matrix_t;

typedef struct _frame_t{
    adjacency_matrix_t &matrix;
    char next;
} frame_t;

adjacency_matrix_t build_adjacency_matrix(vector<string> &cities);
void print_matrix(adjacency_matrix_t &matrix);
void dfs(adjacency_matrix_t &matrix, char target);

int main() 
{
  vector<string> available_cities = read_available_cities();

  vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

  return write_to_file(cities_list);
}

vector<string> combine_cities(vector<string> available_cities) {
  // TODO replace with your solution!
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
    dfs(matrix, 'v'-97);
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

static size_t counter = 0;

void dfs(adjacency_matrix_t &matrix, char target){
    stack<frame_t> stk;
    stk.push({matrix, target});
    string result;

    while(!stk.empty())
    {
        for(char i = 0; i < 26; i++)
        {
            char next = stk.top().next;
            adjacency_matrix_t &mtrx_ref = stk.top().matrix;

            if(mtrx_ref[next][i].path_to_end.size()){
//                mtrx_ref[next][i].path_to_end.push_back('#');
                cout << char(next+65) << char(i+97) << " = " << (mtrx_ref[next][i].path_to_end) << endl;
                next = i;
                continue;
            }
            else if(mtrx_ref[next][i].value)
            {
                mtrx_ref[next][i].value--;
                stk.push({mtrx_ref, i}); // need ref
                result.append({char(next+65), char(i + 97)});

                break;
            }
            else if(i == 25)
            {
                char prev = 0;
                char cur =  0;
                if(result.size()){
                    result.pop_back();
                    cur = result.back() - 65;
                    result.pop_back();
                    prev = *(result.end() - 2) - 65;
                }

//                cout << char(prev+65) << char(cur+65) << char(next + 65)<<endl;
                if(prev > 0) {
                    mtrx_ref[cur][next].path_to_end.insert(mtrx_ref[cur][next].path_to_end.begin(), {char(cur+65), char(next+97)});
                    mtrx_ref[prev][cur].path_to_end.append(mtrx_ref[cur][next].path_to_end);
                }
                stk.pop();
            }
        }
    }
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
        matrix[size_t(word->front() - 65)][size_t(word->back() - 97)].name = word->back();
    }
    return matrix;
}

void print_matrix(adjacency_matrix_t &matrix){
    for(uint8_t i = 'A'; i <= 'Z'; i++){
        for(uint8_t j = 'a'; j <= 'z'; j++){
            cout << matrix[i-65][j-97].value << (matrix[i-65][j-97].value > 9 ? " " : "  ");
        }
        cout << endl;
    }
}
