#pragma once
//Necessaire a cause d'un bug de qt avec des inclusions circulaires meme avec les guards (ifndef)

#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include "weapon.h"

class Character : public Object
{
public:

    int hp;
    bool actionDone;

    Character(string ObjFileName, string TextureFileName, int m, int h, int team);
    void damage(int d, bool t);

    int getTeam(){
        return team;
    }

    int getMovement(){
        return this->movement;
    }

    Weapon* getWeapon(){
        return this->weapon;
    }

protected:
    int movement;
    int team;

    Weapon* weapon;

};

#endif // CHARACTER_H
