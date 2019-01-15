#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include "weapon.h"

class Character : public Object
{
public:
    Character(string ObjFileName, string TextureFileName, int m, int h, int team, Weapon* weapon);
    void damage(int d, bool t);

    int getTeam(){
        return team;
    }

    int getMovement(){
        return movement;
    }

    Weapon* getWeapon(){
            return this->weapon;
        }


    int hp;
    int hpmax;
    bool actionDone=false;

protected:
    int movement;
    int team;

    Weapon* weapon;

};

#endif // CHARACTER_H
