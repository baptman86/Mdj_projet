#include "character.h"

Character::Character(string objFileName, string TextureFileName, int m, int h, int team) : Object (objFileName, TextureFileName), movement(m), hp(h), team(team)
{
}

void Character::damage(int d, bool t){
    if(this->hp-d<=0){
        //TODO kill
    } else {
        this->hp-=d;
    }
}
