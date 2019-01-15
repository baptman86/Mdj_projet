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

MainWidget::MainWidget(int randseed, int fps,unsigned int gridSize, float size, QWidget *parent) :
    QOpenGLWidget(parent),
    fps(fps),
    grid(MapGrid(gridSize,randseed)),
    size(size),
    cursorCoord(make_pair(gridSize/2,gridSize/2)),
    selected(make_pair(-1,-1)),
    selectedObjId(-1),
    turn(1),
    teamTurn(1)
{
    srand (randseed);
    AI *ai = new AI(this->grid);
    this->ai=ai;
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


void MainWidget::select(){
    if(selected.first==-1 || selected.second==-1 || selectedObjId==-1){
        selectedObjId = grid.getData()[cursorCoord.first][cursorCoord.second].ObjId;

        vector<int> cid = grid.charactersId;
        if(std::find(cid.begin(), cid.end(), selectedObjId) != cid.end()){
            for(int i=0;i<grid.getSize();i++){
                for(int j=0;j<grid.getSize();j++){
                    Node player;
                    player.x = cursorCoord.first;
                    player.y = cursorCoord.second;

                    Node destination;
                    destination.x = i;
                    destination.y = j;
                    if(grid.aStar(player, destination,selectedObjId).size()){
                        grid.enlight(i,j);
                    }
                }
            }
            selected = cursorCoord;
        }
    }
    else{
        if(grid.getData()[cursorCoord.first][cursorCoord.second].Surbr){
            grid.setObject(selectedObjId,cursorCoord.first,cursorCoord.second);
            grid.clear();
            selected.first=-1;
            selected.second=-1;
            selectedObjId=-1;
        }
    }
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

    geometries = new GeometryEngine(&grid,size);

    for(int i=0;i<grid.objects.size();i++){
        if(!grid.objects[i]->mesh.loadFromObjFile(QString::fromStdString(string(":/").append(grid.objects[i]->ObjFileName)))){
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

        grid.objects[i]->mesh.texture=new QOpenGLTexture(QImage(QString::fromStdString(string(":/").append(grid.objects[i]->TextureFileName))).mirrored());

        grid.objects[i]->mesh.texture->setMinificationFilter(QOpenGLTexture::Nearest);
        grid.objects[i]->mesh.texture->setMagnificationFilter(QOpenGLTexture::Linear);
        grid.objects[i]->mesh.texture->setWrapMode(QOpenGLTexture::Repeat);
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

    geometries->~GeometryEngine();
    geometries = new GeometryEngine(&grid,size);
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
    program.setUniformValue("a_color", QVector4D(0,1,0,1)*0.4);

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

    programMesh.setUniformValue("v_matrix", QMatrix4x4());
    programMesh.setUniformValue("p_matrix", projection);
    //programMesh.setUniformValue("light_position", QVector3D(light[0], light[1], light[2]));
    //programMesh.setUniformValue("light_color", QVector3D(light[3], light[4], light[5]));
    // Use texture unit 0 which contains red_texture.png
    programMesh.setUniformValue("texture", 0);
    //programMesh.setUniformValue("enable_light", true);
    // Draw cube geometry
    programMesh.setUniformValue("enable_texture", true);


    for(int id : grid.charactersId){

        matrix.setToIdentity();
        matrix.translate(0,0,-1);
        matrix.rotate(rotation);


        matrix.translate((size/(gridSize-1)*grid.objects[id]->getCoord().first+size/(gridSize-1)*(grid.objects[id]->getCoord().first+1))/2-size/2,(size/(gridSize-1)*grid.objects[id]->getCoord().second+size/(gridSize-1)*(grid.objects[id]->getCoord().second+1))/2-size/2,0);


        if(((Character*) grid.objects[id])->getTeam()==0){
            programMesh.setUniformValue("color", QVector4D(1,0,0,1));
            matrix.translate(0.0f,0.15f/gridSize*size,0.0f);
            matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,1),-180));
        }
        else{
            matrix.translate(0.0f,-0.15f/gridSize*size,0.0f);
            programMesh.setUniformValue("color", QVector4D(0,0,1,1));
        }

        matrix.scale(scaling);
        grid.objects[id]->mesh.texture->bind();
        // Set modelview-projection matrix
        programMesh.setUniformValue("m_matrix", matrix);

        grid.objects[id]->mesh.draw(&programMesh);
    }

    programMesh.setUniformValue("color", QVector4D(0,0,0,1));

    for(int id : grid.obstaclesId){
        matrix.setToIdentity();
        matrix.translate(0,0,-1);
        matrix.rotate(rotation);
        matrix.translate(0.0f,-0.3f/gridSize*size,0);


        matrix.translate((size/(gridSize-1)*grid.objects[id]->getCoord().first+size/(gridSize-1)*(grid.objects[id]->getCoord().first+1))/2-size/2,(size/(gridSize-1)*grid.objects[id]->getCoord().second+size/(gridSize-1)*(grid.objects[id]->getCoord().second+1))/2-size/2,0.025f);

        matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0),-90));
        matrix.scale(scaling*1.1);


        grid.objects[id]->mesh.texture->bind();
        // Set modelview-projection matrix
        programMesh.setUniformValue("m_matrix", matrix);

        grid.objects[id]->mesh.draw(&programMesh);
    }
}

bool MainWidget::areAllActionsDone(){
    bool inter = true;
    for(int id : this->grid.charactersId){
        Object *obj=this->grid.objects[id];
        Character *chara = static_cast <Character *>(obj);
        if(this->teamTurn==chara->getTeam()&&chara->actionDone==false){
            inter=false;
        }
    }

    if(inter) {
        this->endTurn();
    }
    return inter;
}

void MainWidget::makeCharacterShoot(int i, pair<int,int> target){
    Object *obj = this->grid.objects[i];
    Character *chara = static_cast <Character *>(obj);

    if(chara->actionDone) {
        cout << "Le personnage a deja agi ce tour" << endl;
    } else {
        //récupère la cible
        int idTarget = this->grid.getData()[target.first][target.second].ObjId;

        Object *obj2 = this->grid.objects[idTarget];
        Character *chara2 = static_cast <Character *>(obj2);

        bool doneSomething = false;

        if(this->grid.isInLosAndRange(chara->getCoord(),target,chara->getWeapon()->getRange())){

            chara2->damage(chara->getWeapon()->getDamage(),chara->getWeapon()->isTerraformer());
            cout << "tir reussi" ;

            doneSomething = true;

        } else {
            cout << "cible hors de vue ou trop loin" << endl;
        }

        if(doneSomething){
            chara->actionDone=true;
            this->areAllActionsDone();
        }

    }
}

void MainWidget::makeCharacterMove(int i, pair<int,int> target){
    Object *obj = this->grid.objects[i];
    Character *chara = static_cast <Character *>(obj);

    if(chara->actionDone) {
        cout << "Le personnage a deja agi ce tour" << endl;
    } else {

        Node player;
        player.x = chara->getCoord().first;
        player.y = chara->getCoord().second;

        Node destination;
        destination.x = target.first;
        destination.y = target.second;

        bool doneSomething = false;

        if(grid.aStar(player, destination,i).size()){
            grid.setObject(i,destination.x,destination.y);
            doneSomething = true;
        } else {
            cout << "Pas de chemin possible";
        }

        if(doneSomething){
            chara->actionDone=true;
            this->areAllActionsDone();
        }

    }
}

void MainWidget::startTurn(){
    //si l'équipe est gérée par une IA, fait agir
    string msg = "Le tour de l'equipe " ;
    msg.append(to_string(this->teamTurn));
    msg.append(" commence.");
    cout << msg  << endl;
    if(this->teamTurn!=0){
        for(int id : this->ai->characterIds){
            Object *obj = grid.objects[id];
            Character *chara = static_cast <Character *>(obj);
            this->ai->currentCharacter=id;

            pair <string, pair<int,int> > aiAnswer = this->ai->act(grid,*chara);
            if(aiAnswer.first=="ACTION_SHOOT"){
                this->makeCharacterShoot(id,aiAnswer.second);
            } else {
//                chara->actionDone=true;
//                this->areAllActionsDone();
                this->makeCharacterMove(id,aiAnswer.second);
            }
        }
    }
}

void MainWidget::endTurn(){
    for(int id : this->grid.charactersId){
        Object *obj = grid.objects[id];
        Character *chara = static_cast <Character *>(obj);

        chara->actionDone=false;
    }
    this->turn++;
    if(this->teamTurn==0){
        this->teamTurn=1;
    } else {
        this->teamTurn=0;
    }
    this->startTurn();
}
