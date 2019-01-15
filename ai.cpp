#include "ai.h"


AI::AI(){

}

void AI::actAll(MapGrid grid){
    for(int id : this->characterIds){
        Object *obj = grid.objects[id];
        Character *chara = static_cast <Character *>(obj);
        this->currentCharacter=id;
        this->act(grid,*chara);
    }
}

void AI::act(MapGrid grid, Character c){
    lookForTarget(grid,c);


    resolve(grid,c);
}

void AI::lookForTarget(MapGrid grid, Character c){
    Character *target;
    bool targetFound = false;
    int maxEnnemyHealth=0;
    for(int id : grid.charactersId){
        Object *obj = grid.objects[id];
        Character *chara = static_cast <Character *>(obj);

        if(chara->getTeam()==0 && grid.isInLosAndRange(c.getCoord(),chara->getCoord(),c.getWeapon()->getRange())){
            if(maxEnnemyHealth==0 || maxEnnemyHealth > chara->hp>maxEnnemyHealth){
                target = chara;
                targetFound = true;
            }
            //ennemies.push_back(*chara);
        }
    }

    if(targetFound){
        this->action="ACTION_SHOOT";
        this->targetCoord=target->getCoord();
    }
}



void AI::resolve(MapGrid grid, Character c){
    if(this->action=="ACTION_SHOOT"){
        grid.makeCharacterShoot(this->currentCharacter, this->targetCoord);
    }

    c.actionDone=true;
}
