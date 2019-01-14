#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include "weapon.h"

class Character : public Object
{
public:
    Character(string ObjFileName, string TextureFileName, int m, int h, int team);
    void damage(int d, bool t);

    int getTeam(){
        return team;
    }

protected:
    int movement;
    int hp;
    int team;

    Weapon* weapon;

};

#endif // CHARACTER_H
