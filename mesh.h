#ifndef MESH_H
#define MESH_H

#include <QVector3D>
#include <QVector2D>
#include <QOpenGLBuffer>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

typedef struct {
    QVector3D position;
    QVector2D textCoord;
    QVector3D normal;
}VertexData;

class Mesh
{


public:
    Mesh();
    QOpenGLBuffer VBO;
    QOpenGLBuffer EBO;

    QOpenGLTexture* texture;

    GLsizei facesCount;

    bool loadFromObjFile(QString filename);
    void draw(QOpenGLShaderProgram* program);

};

#endif // MESH_H
