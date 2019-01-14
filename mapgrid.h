#ifndef GRID_H
#define GRID_H
#include <vector>
#include "astar.h"
#include "object.h"
#include <iostream>

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
    MapGrid(unsigned int size, int randseed);
    unsigned int getSize();
    vector<vector<gridData> > getData();

    bool addCharacter(Object* o);
    bool addCharacter(Object* o, int x, int y);

    bool addObstacle(Object* o);
    bool addObstacle(Object* o, int x, int y);

    bool setObject(int id, int x, int y);

    bool isInLosAndRange( const float x1, const float y1, const float x2, const float y2, int r);
    vector<gridData> findPath(int xStart,int yStart,int xEnd,int yEnd);

    vector<Object*> objects;
    vector<int> charactersId;
    vector<int> obstaclesId;

protected:
    int size;
    vector<vector<gridData> > data;


    float* toWeights();

    vector<pair<int,int>> toGridCoordinates(int* paths);

};

#endif // GRID_H
