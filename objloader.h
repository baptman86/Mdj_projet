#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <QVector2D>
#include <QVector3D>
using namespace std;

class ObjLoader
{
public:
    ObjLoader();
    bool loadOBJ(const char * path, vector < QVector3D > & out_vertices, vector < QVector2D > & out_uvs, vector < QVector3D > & out_normals);
};

#endif // OBJLOADER_H
