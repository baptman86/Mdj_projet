#include "character.h"

Character::Character(string objFileName, int m, int h) : Object (objFileName), movement(m), hp(h)
{
}

void Character::damage(int d, bool t){
    if(this->hp-d<=0){
        //TODO kill
    } else {
        this->hp-=d;
    }
}
