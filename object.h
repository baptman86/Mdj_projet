#ifndef OBJECT_H
#define OBJECT_H
#include "mesh.h"

class Object
{
public:
    Object(string ObjFileName);
    string ObjFileName;
    Mesh mesh;
};

#endif // OBJECT_H
