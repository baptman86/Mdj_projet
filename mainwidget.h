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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <math.h>
#include <sstream>
#include <iostream>
/*#include <thread>
#include <mutex>
#include <condition_variable>*/
#include <QSet>
#include <QTime>
#include <QOpenGLTexture>
#include <QVBoxLayout>
#include <QPushButton>
#include "object.h"
#include "character.h"
#include "obstacle.h"
#include "mapgrid.h"
#include "ai.h"


using namespace std;

class GeometryEngine;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainWidget(int fps=60,unsigned int gridSize = 8, float size=0.5f, QWidget *parent = 0);
    ~MainWidget();

    void addObject(string objFileName);
    void select();

    int fps;
    QQuaternion rotation;
    QQuaternion init_rotation;
    qreal target_angle = 0;
    qreal angle = 0;

    MapGrid grid;

    QSet<Qt::Key> key_pressed;

    pair<int,int> cursorCoord;

    pair<int,int> selected;
    int selectedObjId;

    AI* ai;
    int turn;

    int getTurn(){
        return this->turn;
    }

    void addTurn(){
        this->turn++;
    }

    int teamTurn;

    //Nombre de characters dans une team
    bool areAllActionsDone();

    void makeCharacterShoot(int i, pair<int,int> target);
    void makeCharacterMove(int i, pair<int,int> target);

    //demarre le tour de l'equipe numero t
    void startTurn();
    void endTurn();
protected:
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent *e) override;
    void rotation_handler();
    void move_handler();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

    float size;

    Mesh cursor;

    QVector3D CharacterPosition;
    size_t fps_count; // frames per second
    size_t frames_count;
    size_t ups_count; // updates per second
    size_t updates_count;

    QTime time;

private:

    QBasicTimer timer;
    QOpenGLShaderProgram program;
    QOpenGLShaderProgram programMesh;
    GeometryEngine *geometries;

    QOpenGLTexture *texture;
    QOpenGLTexture *textureCharacter;

    QMatrix4x4 projection;
    QMatrix4x4 projectionTransform;
    QMatrix4x4 modelView;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    qreal angularSpeedDefaultValue = 3;

};

#endif // MAINWIDGET_H
