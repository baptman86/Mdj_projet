#ifndef OBJECT_H
#define OBJECT_H
#include "mesh.h"

class Object
{
public:
    Object(string ObjFileName, string TextureFileName);
    string ObjFileName;
    string TextureFileName;
    Mesh mesh;

    void setCoord(int x,int y){
        this->coord.first=x;
        this->coord.second=y;
    }
    pair<int,int> getCoord(){
        return coord;
    }

protected:
    pair<int,int> coord;
    int id;
};

#endif // OBJECT_H
