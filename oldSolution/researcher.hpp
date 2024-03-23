#ifndef RESEARCHER_HPP
#define RESEARCHER_HPP

#include "ant.hpp"

/* ants researchers use a partial of DFS(PDFS)
 * PDFS permanently remove walked edges */
class researcher : public ant
{
public:
    researcher(matrix &map, edge &start_location);
    ~researcher() final {}

    void move() final;

private:
    vertex *step_back();
};

#endif // RESEARCHER_HPP
