#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stack>

using namespace std;

vector<string> combine_cities(vector<string> available_cities);
vector<string> read_available_cities();
int write_to_file(vector<string> cities_list);

typedef struct _vertex_t{
    uint16_t depth;
    uint16_t value;
}vertex_t;

typedef vector<vertex_t> row_t;
typedef vector<row_t> adjacency_matrix_t;

typedef struct _frame_t{
    adjacency_matrix_t &matrix;
    char target;
} frame_t;

adjacency_matrix_t build_adjacency_matrix(vector<string> &cities);
void print_matrix(adjacency_matrix_t &matrix);
void dfs(stack<frame_t> &stk, string &result);

int main() 
{
  vector<string> available_cities = read_available_cities();

  vector<string> cities_list = combine_cities(available_cities); // TODO implement this function

  return write_to_file(cities_list);
}

vector<string> combine_cities(vector<string> available_cities) {
  // TODO replace with your solution!
    vector<string> test_list(35);
    for(string &i : test_list) i.append({char('A'+rand()%('Z'-'A'+1)), char('a'+rand()%('z'-'a'+1))});
    for(size_t i = test_list.size() - 1; i > 0; i--)
        for(int j = i - 1; j >= 0; j--)
            if(test_list[i].front() < test_list[j].front())
                swap(test_list[i], test_list[j]);
    for(string &i : test_list) cout << i << endl;

    adjacency_matrix_t matrix = build_adjacency_matrix(test_list);
//    print_matrix(matrix);
    stack<frame_t> stack;
    stack.push({matrix, 'v'});
    string str;
    dfs(stack, str);

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

void dfs(stack<frame_t> &stk, string &result){
    while(!stk.empty())
    {
        for(char i = 0; i < 26; i++)
        {
            uint8_t indx = stk.top().target - 97;
            if(stk.top().matrix[indx][i].value)
            {
                char new_target = i + 97;
                stk.top().matrix[indx][i].value--;
                stk.push({stk.top().matrix, new_target});
                result.append({char(indx+65), new_target});
                cout << result << endl;
                break;
            }
            else if(i == 25)
            {
                if(!result.empty()){
                    result.pop_back();
                    result.pop_back();
                }
//                cout << "POP! " << stk.top().target << endl;
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
    }
    return matrix;
}

void print_matrix(adjacency_matrix_t &matrix){
    for(uint8_t i = 'A'; i <= 'Z'; i++){
        for(uint8_t j = 'a'; j <= 'z'; j++){
            cout << i << ":" << j << ":" << matrix[i-65][j-97].value << (matrix[i-65][j-97].value > 9 ? " " : "  ");
        }
        cout << endl;
    }
}
