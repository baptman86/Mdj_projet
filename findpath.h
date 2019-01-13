#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <queue>
#include "mapgrid.h"

class FindPath
{
public:
    FindPath(vector<vector<gridData> > grid, int xStart, int yStart, int xEnd, int yEnd);

private:
    queue<gridData> openList;
    queue<gridData> closedList;
};

#endif // PATHFINDER_H
