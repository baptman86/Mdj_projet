#ifndef GRID_H
#define GRID_H

#include <vector>
#include "object.h"
#include <stack>
#include <iostream>
#include <float.h>

enum GroundType { plante, roche, boue, end };

struct gridData{
    int ObjId;
    GroundType GroundType;
    int Surbr;
};

struct Node
{
    int y;
    int x;
    int parentX;
    int parentY;
    float gCost;
    float hCost;
    float fCost;
};

inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
    return lhs.fCost < rhs.fCost;
}

using namespace std;


class MapGrid
{
public:
    MapGrid(unsigned int size);
    unsigned int getSize();
    vector<vector<gridData> > getData();
    void clearCase(int x, int y){
        data[x][y].ObjId=-1;
    }

    bool addCharacter(Object* o);
    bool addCharacter(Object* o, int x, int y);

    bool addObstacle(Object* o);
    bool addObstacle(Object* o, int x, int y);

    bool setObject(int id, int x, int y);

    bool isInLosAndRange( int x1, int y1, int x2, int y2, int r );
    bool isInLosAndRange( pair<int, int> begin, pair<int,int> end, int r );

    bool isValid(int x, int y, int id, int targetId);
    vector<Node> aStar(Node player, Node dest, int id, int targetId=-1, bool usemouvement=true);
    vector<Node> makePath(vector<vector<Node> > map, Node dest);

    bool plotLineLow(int x0,int y0,int x1,int y1);
    bool plotLineHigh(int x0,int y0,int x1,int y1);
    bool plotLine(int x0,int y0,int x1,int y1);

    void unenlight(int x, int y);
    void enlight(int x, int y);
    void clear();

    vector<Object*> objects;
    vector<int> charactersId;
    vector<int> obstaclesId;

protected:
    int size;
    vector<vector<gridData> > data;

    GroundType randomGround() {
        return static_cast<GroundType>(rand() % GroundType::boue);
    }
    float* toWeights();

    vector<pair<int,int>> toGridCoordinates(int* paths);

};

#endif // GRID_H
