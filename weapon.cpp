#include "weapon.h"

#include "mapgrid.h"

Weapon::Weapon(int r, int d, bool t) : range(r), damage(d), terraformer(t)
{
}

void Weapon::shoot(MapGrid grid, pair<int,int> t){
    this->shoot(grid,t.first,t.second);
}

