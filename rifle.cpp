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
            if(((Character*)grid.objects[objId])->hp-this->getDamage()<=0){
                ((Character*)grid.objects[objId])->hp=0;
                grid.clearCase(((Character*)grid.objects[objId])->getCoord().first,((Character*)grid.objects[objId])->getCoord().second);
                ((Character*)grid.objects[objId])->setCoord(-1,-1);
            }
            else{
                ((Character*)grid.objects[objId])->hp-=this->getDamage();
            }
        }
    }
}
