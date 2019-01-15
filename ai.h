#ifndef AI_H
#define AI_H

#include "character.h"
#include <vector>


class AI
{
public:
    AI();

    void act(MapGrid grid, Character c);
    void actAll(MapGrid grid);



protected:
    vector<int> characterIds;
    int currentCharacter;

    string action;
    pair<int,int> targetCoord;
    void resolve(MapGrid grid, Character c);
    void lookForTarget(MapGrid grid, Character c);

};

#endif // AI_H
