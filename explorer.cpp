#include "explorer.hpp"
#include <algorithm>
#include <iostream>

explorer::explorer(matrix *map, uint8_t location) : ant(map, location) {  }

vertex *explorer::choose_next_vertex()
{
    uint8_t next_location = m_path.back().word.back() - 97;
    vector<vertex*> row_except_empty_vrtx;
    for(vertex &i : personal_map[next_location])
        if(!i.empty())
            row_except_empty_vrtx.push_back(&i);
    return row_except_empty_vrtx.empty() ? nullptr : row_except_empty_vrtx[rand() % row_except_empty_vrtx.size()];
}
