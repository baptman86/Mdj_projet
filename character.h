#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include "weapon.h"

class Character : public Object
{
public:
    Character(string ObjFileName, string TextureFileName, int m, int h, int team);

protected:
    int movement;
    int hp;

    Weapon* weapon;

    void damage(int d, bool t);
};

#endif // CHARACTER_H
