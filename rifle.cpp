#include "rifle.h"

Rifle::Rifle(int d, int r, bool t) : Weapon (d,r,t)
{

}

//void Weapon::shoot(MapGrid grid, int x, int y){

//}

void Rifle::shoot(MapGrid grid, int x, int y){
    if(x>=0 && y>=0 && x < grid.getSize() && y < grid.getSize()){
        int objId = grid.getData()[x][y].ObjId;

        if(!objId<0){
            ((Character*)grid.objects[objId])->damage(this->damage,this->terraformer);
        }
    }
}
