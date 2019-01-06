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

#include "mainwidget.h"

using namespace std;

MainWidget::MainWidget(int fps,std::string img_texture,QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0),
    img_texture(img_texture),
    fps(fps)
{
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

//! [0]
//!

/*mutex mtx;
condition_variable cv;
bool ready = true;
int current = 0;

void rotate(MainWidget* mw,int num, float angle,int milliSecondTime)
{
    current++;
    std::unique_lock<std::mutex> lck(mtx);
    while(!ready){ cv.wait(lck); }
    cout << mw->fps << endl;
    //mw->angularSpeed = angle/milliSecondTime*mw->fps;
    QVector3D n = QVector3D(0.0, 1.0, 1.0).normalized();
    mw->rotation = QQuaternion::fromAxisAndAngle(n, angle) * mw->rotation;
    cout << mw->rotation.x() << " : " << mw->rotation.y() << " : "<< mw->rotation.z() << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliSecondTime));
    //mw->angularSpeed=0;
    cv.notify_all();
}*/


QSet<Qt::Key> key_pressed;

void MainWidget::keyPressEvent(QKeyEvent *e){

    key_pressed << (Qt::Key)e->key();

    if(e->key()==Qt::Key_A){
        /*std::thread t1([this]() { rotate(this, current, 45.0f); });
        t1.detach();*/
        target_angle+=45.0;
    }
    if(e->key()==Qt::Key_E){
        /*std::thread t1([this]() { rotate(this, current, -45.0f); });
        t1.detach();*/
        target_angle-=45.0;
    }
    update();
}

void MainWidget::keyReleaseEvent(QKeyEvent *e){
    key_pressed.remove((Qt::Key)e->key());
}

void MainWidget::wheelEvent(QWheelEvent *event){
    QPoint numDegrees = event->angleDelta()/8;
    if(!numDegrees.isNull()){
        projection.translate(QVector3D(0,0,0.003f*numDegrees.y()));
        update();
    }
}

void display(QMatrix4x4 mat){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            cout << mat.data()[i*4+j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    /*rayCaster->setPosition(e->localPos().toPoint());
    rayCaster->setObjectName("mouseRayCaster");
    rayCaster->setRunMode(rayCaster->SingleShot);
    rayCaster->dumpObjectInfo();*/

    // Save mouse press position
    /*mousePressPosition = QVector2D(e->localPos());
    cout << mousePressPosition.x() << " : " << mousePressPosition.y() << endl;
    float x = (2.0f * mousePressPosition.x()) / this->width() - 1.0f;
    float y = 1.0f - (2.0f * mousePressPosition.y()) / this->height();
    float z = 1.0f;
    QVector3D ray_nds = QVector3D(x, y, z);
    QVector4D ray_clip = QVector4D(ray_nds.x(),ray_nds.y(), -1.0, 1.0);
    QVector4D ray_eye = projection.inverted() * ray_clip;
    ray_eye = QVector4D(ray_eye.x(),ray_eye.y(), -1.0, 0.0);
    QVector3D ray_wor = QVector3D((modelView.inverted() * ray_eye).x(),(modelView.inverted() * ray_eye).y(),(modelView.inverted() * ray_eye).z());
    ray_wor.normalize();
    cout << ray_eye.x() << " : " << ray_eye.y() << " : " << ray_eye.z() << endl;
    cout << ray_wor.x() << " : " << ray_wor.y() << " : " << ray_wor.z() << endl;*/

    //QVector3D viewptr = QVector3D(mousePressPosition.x(),mousePressPosition.y(),0.0);
    //cout << viewptr.unproject(modelView,projection,geometry()).x() << viewptr.unproject(modelView,projection,geometry()).y() << viewptr.unproject(modelView,projection,geometry()).z() << endl;

    /*mousePressPosition = QVector2D(e->localPos());
    QVector3D viewptr = QVector3D(0.0,0.0,0.0);
    QVector3D tmp = viewptr*projection;
    cout << tmp.x() << " : " << tmp.y() << " : " << tmp.z() << endl;;*/

    //display(projection);
    std::vector< QVector3D > vertices;
    std::vector< QVector2D > uvs;
    std::vector< QVector3D > normals; // Won't be used at the moment.
    ObjLoader objloader;
    bool res = objloader.loadOBJ("./nurgle_lowpoly.obj", vertices, uvs, normals);
}

void MainWidget::rotation_handler(){
    // Decrease angular speed (friction)
    QVector3D n = QVector3D(0.0, 1.0f, 1.0f).normalized();
    // Update rotation
    if(target_angle<angle){
        angularSpeed=angularSpeedDefaultValue;
        angle-=angularSpeedDefaultValue;
    }
    else{
        if(target_angle>angle){
            angularSpeed=-angularSpeedDefaultValue;
            angle+=angularSpeedDefaultValue;
        }
        else{
            angularSpeed=0;
        }
    }
    rotation = QQuaternion::fromAxisAndAngle(n, angularSpeed) * rotation;
}

void MainWidget::move_handler(){
    float cameraSpeed = 0.04f;
    float s = sqrt(2);

    if(key_pressed.contains(Qt::Key_Z)){
        projection.translate(QVector3D(0,-cameraSpeed/s,cameraSpeed/s));
    }
    if(key_pressed.contains(Qt::Key_S)){
        projection.translate(QVector3D(0,cameraSpeed/s,-cameraSpeed/s));
    }
    if(key_pressed.contains(Qt::Key_D)){
        projection.translate(QVector3D(-cameraSpeed,0,0));
    }
    if(key_pressed.contains(Qt::Key_Q)){
        projection.translate(QVector3D(cameraSpeed,0,0));
    }
}

void MainWidget::timerEvent(QTimerEvent *e)
{
    rotation_handler();
    move_handler();
    update();
}

//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    geometries = new GeometryEngine;


    // Use QBasicTimer because its faster than QTimer
    timer.start(1000/fps, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/groundSquart.png"));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.001, zFar = 50.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    QVector3D eye = QVector3D(1.0, 0.0, 0.0);
    init_rotation = QQuaternion::fromAxisAndAngle(eye, -45.0f);
    rotation = init_rotation;

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
    QVector3D centerOfInterest(0, 0, 0), up(1, 0, 0);
    modelView.setToIdentity();
    modelView.lookAt(eye, centerOfInterest, up);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -1.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);
}
