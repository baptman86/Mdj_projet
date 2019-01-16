#ifndef AI_H
#define AI_H

#include "character.h"
#include <vector>


class AI
{
public:
    AI(MapGrid grid);

    pair <string, pair<int,int> > act(MapGrid grid, Character* c);
    void actAll(MapGrid grid);

    vector<int> characterIds;
    int currentCharacter;

protected:


    string action;
    pair<int,int> targetCoord;
    void resolve(MapGrid grid, Character* c);
    bool lookForTarget(MapGrid grid, Character* c);

    pair<int,int> goToClosestTarget(MapGrid grid, Character* c);

};

#endif // AI_H
