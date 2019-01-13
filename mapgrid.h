#ifndef GRID_H
#define GRID_H
#include <vector>
#include "astar.cpp"
#include "object.h"

enum GroundType { plante, boue, roche };
enum SurbrType { rien, brillant };

typedef struct gridData{
    int ObjId;
    GroundType GroundType;
    SurbrType SurbrType;
} gridData;


using namespace std;


class MapGrid
{
public:
    MapGrid(unsigned int size);
    unsigned int getSize();
    vector<vector<gridData> > getData();

    Object findObjectById(int id);
    //assume que l'objet est bien present (a checker avec isObjectById(int))

    bool isObjectById(int id);

    bool addObject(Object o);
    bool addObject(Object o, int x, int y);
    bool setObject(int id, int x, int y);

    bool isInLosAndRange( const float x1, const float y1, const float x2, const float y2, int r);
    vector<gridData> findPath(int xStart,int yStart,int xEnd,int yEnd);

protected:
    int size;
    vector<vector<gridData> > data;
    //vector<Node> toNodes();
    vector< pair< int,Object> > objects;


    float* toWeights();

    vector<pair<int,int>> toGridCoordinates(int* paths);

};

#endif // GRID_H
