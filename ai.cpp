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

pair <string, pair<int,int> > AI::act(MapGrid grid, Character *c){

    cout << "AI ENGAGED" << endl;
    //si pas d'arme
    if(c->getWeapon()==nullptr){
        cout << "moi " << c->ObjFileName << ",j'ai pas d'arme lol" << endl ;
        this->action="derp";
        this->targetCoord=make_pair(0,0);

    //Si trouve une cible shootable
    } else if(!lookForTarget(grid,c)){

        //Sinon se rapproche au minimum pour etre a portee
        this->action="ACTION_MOVE";
        this->targetCoord=this->goToClosestTarget(grid,c);
    }

    return make_pair(this->action,this->targetCoord);
}

bool AI::lookForTarget(MapGrid grid, Character *c){

    Character *target;
    bool targetFound = false;
    int maxEnnemyHealth=0;
    for(int id : grid.charactersId){
        Object *obj = grid.objects[id];
        Character *chara = static_cast <Character *>(obj);

        if(chara->getCoord().first<0 || chara->getCoord().second<0 ){
            continue;
        }

        if(chara->getTeam()==0 && grid.isInLosAndRange(c->getCoord(),chara->getCoord(),c->getWeapon()->getRange())){
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

    return targetFound;

}

pair<int,int> AI::goToClosestTarget(MapGrid grid, Character *c){
    Character *target;
    int idSelf;

    pair<int,int> result;

    for(int j=0;j<grid.objects.size();j++){
        string msg = "Le tour de l'equipe " ;
        msg.append(" commence.");
        cout << msg  << endl;
        if(c==grid.objects[j]){
            idSelf=j;
            break;
        }
    }

    bool targetFound = false;
    int min=-1;

    int targetX;
    int targetY;

    vector<Node> goodPath;

    for(int id : grid.charactersId){
        Object *obj = grid.objects[id];
        Character *chara = static_cast <Character *>(obj);

        if(chara->getTeam()==0 && !(chara->getCoord().first<0 || chara->getCoord().second<0)){

        Node begin, end;

        begin.x=c->getCoord().first;
        begin.y=c->getCoord().second;

        end.x=chara->getCoord().first;
        end.y=chara->getCoord().second;


            vector<Node> longPath=grid.aStar(begin, end, idSelf, id,false);
            if(min==-1 || min >longPath.size()){
                min = longPath.size();
                target = chara;
                targetFound = true;
                goodPath=longPath;
                targetX=end.x;
                targetY=end.y;
            }
        }
    }

    const int maxSize=goodPath.size();

    //Si le mouvement permet d'arriver à portée avec la valeur de mouvement
    if(c->getWeapon()->getRange()+c->getMovement() < maxSize){

        int distanceToGo=goodPath.size()-c->getWeapon()->getRange();

        //            for(int ji = 0; ji<distanceToGo; ji++){
        //                result.insert(result.begin(),goodPath[ji]);
        //            }

        //S'il y a un problème d'overflow, c'est par ici
        //Check si la distance minimum suffit à pouvoir tirer
        while(!grid.isInLosAndRange(goodPath[distanceToGo].x,goodPath[distanceToGo].y,targetX,targetY,c->getWeapon()->getRange())
              && distanceToGo+1<c->getMovement()-1){

            distanceToGo++;
            //result.insert(result.begin(),goodPath[distanceToGo]);

        }
        for(int ij = 0 ; ij < distanceToGo ; ij++){
                    if(goodPath[ij].gCost>c->getMovement()){
                            distanceToGo=ij-1;
                            break;
                    }
                }
                result.first = goodPath[distanceToGo].x;
                result.second = goodPath[distanceToGo].y;
            } else {

                int m=c->getMovement()-1;
                for(int ij = 0 ; ij < m ; ij++){
                    if(goodPath[ij].gCost>m){
                            m=ij-1;
                            break;
                    }
                }
                result.first = goodPath[m].x;
                result.second = goodPath[m].y;
            }



            return result;

}



//string AI::resolve(MapGrid grid, Character c){
//    if(this->action=="ACTION_SHOOT"){
//        return (action);
//    }

//    c->actionDone=true;
//}
