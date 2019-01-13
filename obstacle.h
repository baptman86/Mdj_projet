#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "object.h"


class Obstacle : public Object
{
public:
    Obstacle(string ObjFileName);

    void damage(int d, bool t){
        if(t){
            //TODO kill
        }
    }
};

#endif // OBSTACLE_H
