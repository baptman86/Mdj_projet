#ifndef OBJECT_H
#define OBJECT_H
#include "mesh.h"

class Object
{
public:
    Object(string ObjFileName);
    string ObjFileName;
    Mesh mesh;

    void damage(int d, bool t); //TODO : les kills

    void setCoord(int x,int y){
        this->coord.first=x;
        this->coord.second=y;
    }
    pair<int,int> getCoord(){
        return coord;
    }

protected:
    pair<int,int> coord;
};

#endif // OBJECT_H
