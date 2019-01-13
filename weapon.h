#ifndef WEAPON_H
#define WEAPON_H

#include "mapgrid.h"

class Weapon
{
public:
    Weapon(int r, int d, bool t);

    virtual void shoot(MapGrid grid,int x, int y)=0;

protected:
    int range;
    int damage;
    bool terraformer;

    int getRange() const{
        return this->range;
    }

    int getDamage() const{
        return this->damage;
    }

    bool isTerraformer() const{
        return this->terraformer;
    }
};

#endif // WEAPON_H
