#include "rifle.h"

Rifle::Rifle(int d, int r, bool t) : Weapon (d,r,t)
{

}

//void Weapon::shoot(MapGrid grid, int x, int y){

//}

void Rifle::shoot(MapGrid grid, int x, int y){
    int objId = grid.getData()[x][y].ObjId;

    if(objId!=0){
        if(grid.isObjectById(objId)){
            Object target = grid.findObjectById(objId);

            target.damage(this->damage,this->terraformer);

        }
    }
}
