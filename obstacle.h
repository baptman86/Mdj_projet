#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "object.h"


class Obstacle : public Object
{
public:
    Obstacle(string ObjFileName, string TextureFileName);
    void damage(int d, bool t);
};

#endif // OBSTACLE_H
