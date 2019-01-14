/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <stdlib.h>
#include <QImage>
#include <QColor>
#include <sstream>
#include <iostream>


struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine(MapGrid* mapGrid,float size)
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    this->mapGrid = mapGrid;

    // Initializes plane geometry and transfers it to VBOs
    initPlaneGeometry(size);


}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]


//init data of a plane with size² vertex, starting from point (x,y), height of h et width of w. create a randomized z component for each vertices between 0 and z_max. texture_x and texture_y are exclusives of cube.png and give starting point to get texture from this file.
void GeometryEngine::initPlaneGeometry(float size)
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    int gridSize = mapGrid->getSize();

    float x = -size/2, y = -size/2;
    int i_indices = 0;
    float i_img, j_img;

    VertexData *vertices = new VertexData[gridSize*gridSize*4];
    GLushort *indices = new GLushort[gridSize*gridSize*6];

    for(int i=0; i<gridSize*gridSize*4; i+=4){

        i_img = ((float) ( (i/4) % gridSize ) );
        j_img = ((float) ( (i/4) / gridSize ) );

        int texture;

        switch (mapGrid->getData()[i_img][j_img].GroundType) {
            case plante :
                texture = 1;
                break;
            case boue :
                texture = 2;
                break;
            case roche :
                texture = 0;
                break;
            default :
                texture = 1;
                break;
        }

        vertices[i] =   {QVector3D( x+(i_img/(gridSize-1))*size, y+(j_img/(gridSize-1))*size,  0 ), QVector2D(1.0f/3*texture, 0.0f)};  // v0
        vertices[i+1] = {QVector3D( x+((i_img+1)/(gridSize-1))*size , y+(j_img/(gridSize-1))*size,  0 ), QVector2D(1.0f/3*(texture+1), 0.0f)}; // v1
        vertices[i+2] = {QVector3D( x+(i_img/(gridSize-1))*size, y+((j_img+1)/(gridSize-1))*size ,  0 ), QVector2D(1.0f/3*texture, 1.0f)};  // v2
        vertices[i+3] = {QVector3D( x+((i_img+1)/(gridSize-1))*size  , y+((j_img+1)/(gridSize-1))*size ,  0 ), QVector2D(1.0f/3*(texture+1), 1.0f)}; // v3


        indices[i_indices] = i;
        indices[i_indices+1] = i+1;
        indices[i_indices+2] = i+2;

        indices[i_indices+3] = i+1;
        indices[i_indices+4] = i+3;
        indices[i_indices+5] = i+2;

        i_indices+=6;
    }


//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, gridSize*gridSize*4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, gridSize*gridSize*6 * sizeof(GLushort));
//! [1]
}

//! [2]

void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, 10000000, GL_UNSIGNED_SHORT, 0);
}
