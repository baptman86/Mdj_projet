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

MainWidget::MainWidget(int fps,unsigned int gridSize, float size, QWidget *parent) :
    QOpenGLWidget(parent),
    fps(fps),
    grid(MapGrid(gridSize)),
    size(size),
    cursorCoord(make_pair(gridSize/2,gridSize/2))
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
        target_angle-=45.0;
    }
    if(e->key()==Qt::Key_E){
        /*std::thread t1([this]() { rotate(this, current, -45.0f); });
        t1.detach();*/
        target_angle+=45.0;
    }

    if(e->key()==Qt::Key_Up){
        /*std::thread t1([this]() { rotate(this, current, -45.0f); });
        t1.detach();*/
        cursorCoord.second++;
    }
    if(e->key()==Qt::Key_Down){
        /*std::thread t1([this]() { rotate(this, current, -45.0f); });
        t1.detach();*/
        cursorCoord.second--;
    }
    if(e->key()==Qt::Key_Right){
        /*std::thread t1([this]() { rotate(this, current, -45.0f); });
        t1.detach();*/
        cursorCoord.first++;
    }
    if(e->key()==Qt::Key_Left){
        /*std::thread t1([this]() { rotate(this, current, -45.0f); });
        t1.detach();*/
        cursorCoord.first--;
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


int euh=0;
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    grid.setObject(0,euh,euh);
    euh++;
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
    float cameraSpeed = 0.02f;
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
    if(key_pressed.contains(Qt::Key_Plus)){
        projection.translate(QVector3D(0,0,cameraSpeed));
    }
    if(key_pressed.contains(Qt::Key_Minus)){
        projection.translate(QVector3D(0,0,-cameraSpeed));
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

    geometries = new GeometryEngine(grid.getSize(),0.5f);

    for(int i=0;i<grid.objects.size();i++){
        if(!grid.objects[i].mesh.loadFromObjFile(QString::fromStdString(string(":/").append(grid.objects[i].ObjFileName)))){
            std::cerr<<"Unable to load the .obj file"<<std::endl;
        }
    }

    if(!cursor.loadFromObjFile(":/selector.obj")){
        std::cerr<<"Unable to load the cursor .obj file"<<std::endl;
    }


    // Use QBasicTimer because its faster than QTimer
    timer.start(1000/fps, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl")){
        std::cerr << "Error while compiling vertex shader: " << program.log().toStdString() << std::endl;
        exit(1);
    }
    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl")){
        std::cerr << "Error while compiling fragment shader: " << program.log().toStdString() << std::endl;
        exit(1);
    }
    // Link shader pipeline
    if (!program.link()){
        std::cerr << "Error while link fragment shader: " << program.log().toStdString() << std::endl;
        exit(1);
    }
    //MESH
    // Compile vertex shader
    if (!programMesh.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshadermesh.vert")){
        std::cerr << "Error while compiling vertex shader mesh: " << programMesh.log().toStdString() << std::endl;
        exit(1);
    }

    // Compile fragment shader
    if (!programMesh.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshadermesh.frag")){
        std::cerr << "Error while compiling fragment shader mesh: " << programMesh.log().toStdString() << std::endl;
        exit(1);
    }

    // Link shader pipeline
    if (!programMesh.link()){
        std::cerr << "Error while link fragment shader mesh: " << programMesh.log().toStdString() << std::endl;
        exit(1);
    }

}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load groundSquart.png image
    texture = new QOpenGLTexture(QImage(":/groundSquart.png"));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
    for(int i=0;i<grid.objects.size();i++){

        grid.objects[i].mesh.texture=new QOpenGLTexture(QImage(QString::fromStdString(string(":/").append(grid.objects[i].TextureFileName))).mirrored());

        grid.objects[i].mesh.texture->setMinificationFilter(QOpenGLTexture::Nearest);
        grid.objects[i].mesh.texture->setMagnificationFilter(QOpenGLTexture::Linear);
        grid.objects[i].mesh.texture->setWrapMode(QOpenGLTexture::Repeat);
    }

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
    /*QVector3D centerOfInterest(0, 0, 0), up(1, 0, 0);
    modelView.setToIdentity();
    modelView.lookAt(eye, centerOfInterest, up);*/
}
//! [5]


void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -1.0);
    matrix.rotate(rotation);

    //////////////CAMERA
    const float x = 1.0f;
    const float y = 5.0f;
    const float z = 1.0f;
    QVector3D eye = QVector3D(x, y, z); //position de la camera
    QVector3D targetCamera = QVector3D(CharacterPosition); //cible de la camera
    QVector3D up = QVector3D(1.0f, 0.0f, 0.0f); //permet une stabilité de la caméra
    QMatrix4x4 view;
    view.lookAt(eye, targetCamera, up);
    float light[]={
       0.0f,0.0f,0.0f, //positon light
       1.0f,1.0f,1.0f  //color light
    };

    /////////////////////TERRAIN
    texture->bind();

    if (!program.bind()){
        std::cerr << "(MainWidget::paintGL Error while binding fragment shader: " << program.log().toStdString() << std::endl;
        exit(1);
    }

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);

    ////////////////////////MESHS

    int gridSize = grid.getSize();
    QVector3D scaling = QVector3D(1.0f/gridSize*size,1.0f/gridSize*size,1.0f/gridSize*size);

    ////////////////////////MESH DU CURSEUR

    programMesh.bind();
    programMesh.setUniformValue("enable_texture", false);
    programMesh.setUniformValue("color", QVector4D(1,0,0,1));

    matrix.setToIdentity();
    matrix.translate(0,0,-1);
    matrix.rotate(rotation);

    matrix.translate((size/(gridSize-1)*cursorCoord.first+size/(gridSize-1)*(cursorCoord.first+1))/2-size/2,(size/(gridSize-1)*cursorCoord.second+size/(gridSize-1)*(cursorCoord.second+1))/2-size/2,0);
    matrix.scale(scaling*0.5f);

    programMesh.setUniformValue("m_matrix", matrix);

    cursor.texture=nullptr;
    cursor.draw(&programMesh);

    ////////////////////////MESH DES OBJETS
    ///
    programMesh.setUniformValue("v_matrix", QMatrix4x4());
    programMesh.setUniformValue("p_matrix", projection);
    //programMesh.setUniformValue("light_position", QVector3D(light[0], light[1], light[2]));
    //programMesh.setUniformValue("light_color", QVector3D(light[3], light[4], light[5]));
    // Use texture unit 0 which contains red_texture.png
    programMesh.setUniformValue("texture", 0);
    //programMesh.setUniformValue("enable_light", true);
    // Draw cube geometry
    programMesh.setUniformValue("enable_texture", true);





    for(int i=0;i<grid.objects.size();i++){
        matrix.setToIdentity();
        matrix.translate(0,0,-1);
        matrix.rotate(rotation);
        matrix.translate(0.0f,-0.15f/gridSize*size,0.0f);


        matrix.translate((size/(gridSize-1)*grid.objects[i].getCoord().first+size/(gridSize-1)*(grid.objects[i].getCoord().first+1))/2-size/2,(size/(gridSize-1)*grid.objects[i].getCoord().second+size/(gridSize-1)*(grid.objects[i].getCoord().second+1))/2-size/2,0);
        matrix.scale(scaling);


        grid.objects[i].mesh.texture->bind();
        // Set modelview-projection matrix
        programMesh.setUniformValue("m_matrix", matrix);

        grid.objects[i].mesh.draw(&programMesh);
    }
}
