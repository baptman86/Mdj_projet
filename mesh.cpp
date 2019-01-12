#include "mesh.h"
#include <fstream>
#include <iostream>
#include <string>
#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <vector>
#include <chrono>

Mesh::Mesh() : VBO(QOpenGLBuffer()), EBO(QOpenGLBuffer::IndexBuffer)
{

}

std::ostream& operator<< (std::ostream &out, QVector3D &v){
    out << v.x() << " " << v.y() << " " << v.z() << std::endl;
    return out;
}

bool Mesh::loadFromObjFile(QString filename)
{
    if(!VBO.create())
    {
        std::cerr << "Error creating VBO" << std::endl;
    }

    if(!VBO.bind())
    {
        std::cerr << "Error binding array buffer" << std::endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<VertexData> vertices;

    std::string line;
    std::cout << "Trying to open file: " << filename.toStdString() << std::endl;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);

    unsigned int verticesCount = 0,
                 normalsCount = 0,
                 textCoordsCount = 0;

    facesCount = 0;

    QString type;
    char separator;
    unsigned int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3, v4, vt4, vn4;

    while(!in.atEnd())
    {
        in >> type;
        if (type == "v")
        {
            verticesCount++;
        }
        else if (type == "vn")
        {
            normalsCount++;
        }
        else if (type == "vt")
        {
            textCoordsCount++;
        }
        else if (type == "f")
        {
            facesCount++;
        }
        in.readLine();
    }

    in.seek(0);

    float* verticesData = new float[verticesCount * 3];
    float* normalsData = new float[normalsCount * 3];
    float* textCoordsData = new float[textCoordsCount * 2];

    verticesCount = 0;
    textCoordsCount = 0;
    normalsCount = 0;

    while(!in.atEnd())
    {
        in >> type;
        if (type == "v")
        {
            in >> verticesData[verticesCount] >> verticesData[verticesCount+1] >> verticesData[verticesCount+2];
            verticesCount += 3;
        }
        else if (type == "vn")
        {
            in >> normalsData[normalsCount] >> normalsData[normalsCount+1] >> normalsData[normalsCount+2];
            normalsCount += 3;
        }
        else if (type == "vt")
        {
            in >> textCoordsData[textCoordsCount] >> textCoordsData[textCoordsCount+1];
            textCoordsCount += 2;
            in.readLine();
        }
        else if (type == "f")
        {
            in >> v1 >> separator >> vt1 >> separator >> vn1
               >> v2 >> separator >> vt2 >> separator >> vn2
               >> v3 >> separator >> vt3 >> separator >> vn3
               >> v4 >> separator >> vt4 >> separator >> vn4;


            v1--; v2--; v3--; v4--;
            vt1--; vt2--; vt3--; vt4--;
            vn1--; vn2--; vn3--; vn4--;


            vertices.push_back({
                QVector3D(verticesData[v1*3], verticesData[v1*3+1], verticesData[v1*3+2]),
                QVector2D(textCoordsData[vt1*2], textCoordsData[vt1*2+1]),
                QVector3D(normalsData[vn1*3], normalsData[vn1*3+1], normalsData[vn1*3+2])
            });

            vertices.push_back({
                QVector3D(verticesData[v2*3], verticesData[v2*3+1], verticesData[v2*3+2]),
                QVector2D(textCoordsData[vt2*2], textCoordsData[vt2*2+1]),
                QVector3D(normalsData[vn2*3], normalsData[vn2*3+1], normalsData[vn2*3+2])
            });

            vertices.push_back({
                QVector3D(verticesData[v3*3], verticesData[v3*3+1], verticesData[v3*3+2]),
                QVector2D(textCoordsData[vt3*2], textCoordsData[vt3*2+1]),
                QVector3D(normalsData[vn3*3], normalsData[vn3*3+1], normalsData[vn3*3+2])
            });

            vertices.push_back({
                QVector3D(verticesData[v4*3], verticesData[v4*3+1], verticesData[v4*3+2]),
                QVector2D(textCoordsData[vt4*2], textCoordsData[vt4*2+1]),
                QVector3D(normalsData[vn4*3], normalsData[vn4*3+1], normalsData[vn4*3+2])
            });


        }
    }

    VBO.allocate(vertices.data(), vertices.size() * sizeof(VertexData));

    delete[] verticesData;
    delete[] normalsData;
    delete[] textCoordsData;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "file " << filename.toStdString() << " loaded in " << elapsed_seconds.count() << " second(s) ("<< vertices.size() << " vertices)." << std::endl;
    file.close();
    return true;
}


void Mesh::draw(QOpenGLShaderProgram* program)
{
    if(!VBO.bind())
    {
        std::cerr << "Error binding array buffer" << std::endl;
    }

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, sizeof(QVector3D), 2, sizeof(VertexData));

    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, sizeof(QVector3D) + sizeof(QVector2D), 3, sizeof(VertexData));

    if(texture != nullptr)
    {
        texture->bind();
    }
    glDrawArrays(GL_QUADS, 0, facesCount * 4);

}
