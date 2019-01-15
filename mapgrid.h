#pragma once
//Necessaire a cause d'un bug de qt avec des inclusions circulaires meme avec les guards (ifndef)

#ifndef GRID_H
#define GRID_H

#include <vector>
#include "object.h"
#include <stack>
#include <iostream>

enum GroundType { plante, boue, roche, end };

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

    bool isValid(int x, int y, int id);
    vector<Node> aStar(Node player, Node dest, int id);
    vector<Node> makePath(vector<vector<Node> > map, Node dest);

    void unenlight(int x, int y);
    void enlight(int x, int y);
    void clear();

    vector<Object*> objects;
    vector<int> charactersId;
    vector<int> obstaclesId;

    bool isInLosAndRange( float x1, float y1, float x2, float y2, int r );
    bool isInLosAndRange( pair<int,int> begin, pair<int,int> end, int r );


protected:

    int size;

    vector<vector<gridData> > data;

    GroundType randomGround() {
        return static_cast<GroundType>(rand() % GroundType::end);
    }
    float* toWeights();

    vector<pair<int,int>> toGridCoordinates(int* paths);




};

#endif // GRID_H
