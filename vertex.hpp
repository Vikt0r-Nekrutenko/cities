#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <string>

using std::vector;
using std::string;

typedef struct _edge_t{
    string word;
    uint16_t index;
    float pheromone;
} edge_t;

class vertex : public vector<edge_t>
{
public:
    void print() const;
    void sort();
};

#endif // VERTEX_HPP
