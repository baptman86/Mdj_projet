#include "widget.h"

Widget::Widget(int fps,unsigned int gridSize, float size, QWidget *parent)
{
    mainWidget = new MainWidget(fps,gridSize,size,this);

    endTurnButton = new QPushButton("End Turn");


    ui = new QHBoxLayout;

    endTurnButton->installEventFilter(this);

    ui->addWidget(endTurnButton);

    QVBoxLayout *assemble = new QVBoxLayout;
    assemble->addWidget(mainWidget);
    assemble->addLayout(ui);

    this->focusPolicy();
    this->setLayout(assemble);
    timer.start(500,this);
}


bool Widget::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        if(e->key()==Qt::Key_Up){
            if(mainWidget->cursorCoord.second<(mainWidget->grid.getSize()-1)){
                mainWidget->cursorCoord.second++;
            }
        }
        if(e->key()==Qt::Key_Down){
            if(mainWidget->cursorCoord.second>0){
                mainWidget->cursorCoord.second--;
            }
        }
        if(e->key()==Qt::Key_Right){
            if(mainWidget->cursorCoord.first<(mainWidget->grid.getSize()-1)){
                mainWidget->cursorCoord.first++;
            }
        }
        if(e->key()==Qt::Key_Left){
            if(mainWidget->cursorCoord.first>0){
                mainWidget->cursorCoord.first--;
            }
        }
        mainWidget->key_pressed << (Qt::Key)e->key();

        if(e->key()==Qt::Key_Enter-1){
            const int selectedId = mainWidget->selectedObjId;
            if(selectedId!=-1){
                int x = mainWidget->cursorCoord.first;
                int y = mainWidget->cursorCoord.second;

                int targetId = mainWidget->grid.getData()[x][y].ObjId;

                if(targetId<0){
                    this->mainWidget->makeCharacterMove(selectedId,make_pair(x,y));
                } else {
                    this->mainWidget->makeCharacterShoot(selectedId,make_pair(x,y));
                }
                mainWidget->selectedObjId=-1;
                mainWidget->grid.clear();
            } else {
                mainWidget->select();
            }
        }
        if(e->key()==Qt::Key_Escape){
            mainWidget->grid.clear();
            mainWidget->selected.first=-1;
            mainWidget->selected.second=-1;
            mainWidget->selectedObjId=-1;

        }
        return true;
    }
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        mainWidget->key_pressed.remove((Qt::Key)e->key());
    }
    if(event->type() == QEvent::MouseButtonPress){
        if(object == endTurnButton){
            mainWidget->endTurn();
        }
    }
    return false;
}

void Widget::timerEvent(QTimerEvent *e)
{
    displayCharacter();
    endGame();
    update();
}

void Widget::displayCharacter(){
    for(QVBoxLayout* l : characterInfos){
        ui->removeItem(l);
        for(int i=0;i<l->count();i++){
            l->takeAt(i)->widget()->hide();
            delete l->takeAt(i)->widget();
        }
        delete l;
    }
    characterInfos.clear();
    for(int id : mainWidget->grid.charactersId){
        string color = "rgba( 160, 0, 0, 255)";
        if(((Character*)mainWidget->grid.objects[id])->getTeam()!=0){
            color = "rgba( 80, 80, 160, 255)";
        }


        QLabel *label = new QLabel();
        label->setText(string("soldierRifle").append(to_string(id)).c_str());
        label->setStyleSheet( "QWidget{ background-color : "+QString::fromStdString(color)+"; border-radius : 2px;  }" );
        label->setFixedHeight(15);
        label->setWordWrap(true);
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(label);

        QProgressBar* pb = new QProgressBar();
        pb->setRange(0,((Character*)mainWidget->grid.objects[id])->hpmax);
        pb->setValue(((Character*)mainWidget->grid.objects[id])->hp);
        pb->setStyleSheet( "QWidget{ background-color : "+QString::fromStdString(color)+"; border-radius : 2px;  }" );
        layout->addWidget(pb);

        characterInfos.push_back(layout);
        ui->addLayout(layout);
    }
}

void Widget::endGame(){
    if(mainWidget->turn>0){
        bool team0 = true;
        bool team1 = true;
        for(int id : mainWidget->grid.charactersId){
            pair<int,int> coord = mainWidget->grid.objects[id]->getCoord();
            if(coord.first!=-1 || coord.second!=-1){
                if(((Character*)mainWidget->grid.objects[id])->getTeam()==0){
                    team0=false;
                }
                else{
                    team1=false;
                }
            }
        }
        if(team0){
            timer.stop();
            QHBoxLayout *tmp = new QHBoxLayout;
            tmp->addWidget(new QLabel("Vous avez Perdu !"));
            QWidget* w = new QWidget();
            w->setLayout(tmp);
            w->show();
            this->hide();
        }
        if(team1){
            timer.stop();
            QHBoxLayout *tmp = new QHBoxLayout;
            tmp->addWidget(new QLabel("Vous avez gagné !"));
            QWidget* w = new QWidget();
            w->setLayout(tmp);
            w->show();
            this->hide();
        }
    }
}
