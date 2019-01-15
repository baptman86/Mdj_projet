#include "ai.h"


AI::AI(MapGrid grid){
    for(int id : grid.charactersId){
        Object *obj = grid.objects[id];
        Character *chara = static_cast <Character *>(obj);
        if(chara->getTeam()!=0){
            this->characterIds.push_back(id);
        }
    }
}

//void AI::actAll(MapGrid grid){
//    for(int id : this->characterIds){
//        Object *obj = grid.objects[id];
//        Character *chara = static_cast <Character *>(obj);
//        this->currentCharacter=id;
//        this->act(grid,*chara);
//    }
//}

pair <string, pair<int,int> > AI::act(MapGrid grid, Character c){
    if(c.getWeapon()==nullptr){
        cout << "moi " << c.ObjFileName << ",j'ai pas d'arme lol" ;
        this->action="derp";
        this->targetCoord=make_pair(0,0);
    } else {
        lookForTarget(grid,c);
    }
    return make_pair(this->action,this->targetCoord);
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



//string AI::resolve(MapGrid grid, Character c){
//    if(this->action=="ACTION_SHOOT"){
//        return (action);
//    }

//    c.actionDone=true;
//}
