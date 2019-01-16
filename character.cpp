#include "character.h"

Character::Character(string objFileName, string TextureFileName, int m, int h, int team, Weapon* w) : Object (objFileName, TextureFileName), movement(m), hp(h), team(team), hpmax(hp), weapon(w)
{
}

void Character::damage(int d, bool t){
    if(this->hp-d<=0){
        this->setCoord(-1,-1);
        this->hp=0;
    } else {
        this->hp-=d;
    }
}
