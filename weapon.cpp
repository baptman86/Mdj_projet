#include "weapon.h"

Weapon::Weapon(int d, int r, bool t) : range(r), damage(d), terraformer(t)
{
}

void Weapon::shoot(MapGrid grid, pair<int,int> t){
    this->shoot(grid,t.first,t.second);
}
