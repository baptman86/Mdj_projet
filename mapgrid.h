#ifndef GRID_H
#define GRID_H
#include <vector>

typedef struct gridData{
    int ObjId;
    int GroundType;
    int SurbrType;
} gridData;

using namespace std;


class MapGrid
{
public:
    MapGrid(unsigned int size);
    unsigned int getSize();
    vector<vector<gridData> > getData();

protected:
    int size;
    vector<vector<gridData> > data;
};

#endif // GRID_H
