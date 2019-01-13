#ifndef RIFLE_H
#define RIFLE_H

#include "weapon.h"
#include "character.h"


class Rifle : virtual public Weapon
{
public:
    Rifle(int d, int r, bool t);
    void shoot(MapGrid grid, int x, int y) override;
};

#endif // RIFLE_H
