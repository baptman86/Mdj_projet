#pragma once
//Necessaire a cause d'un bug de qt avec des inclusions circulaires meme avec les guards (ifndef)

#ifndef WEAPON_H
#define WEAPON_H

#include "mapgrid.h"

class Weapon
{
public:
    Weapon(int r, int d, bool t);

    virtual void shoot(MapGrid grid,int x, int y)=0;
    void shoot(MapGrid grid,pair<int,int> t);

    int getRange() const{
        return this->range;
    }

    int getDamage() const{
        return this->damage;
    }

    bool isTerraformer() const{
        return this->terraformer;
    }

protected:
    int range;
    int damage;
    bool terraformer;


};

#endif // WEAPON_H
