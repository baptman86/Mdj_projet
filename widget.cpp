#include "widget.h"

Widget::Widget(int randseed, int fps,unsigned int gridSize, float size, QWidget *parent)
{
    mainWidget = new MainWidget(randseed,fps,gridSize,size,this);
    moveButton = new QPushButton("Move");
    shootButton = new QPushButton("Shoot");
    endTurnButton = new QPushButton("End Turn");

    moveButton->installEventFilter(this);
    shootButton->installEventFilter(this);
    endTurnButton->installEventFilter(this);

    QHBoxLayout *ui = new QHBoxLayout;
    ui->addWidget(moveButton);
    ui->addWidget(shootButton);
    ui->addWidget(endTurnButton);

    QVBoxLayout *assemble = new QVBoxLayout;
    assemble->addWidget(mainWidget);
    assemble->addLayout(ui);

    this->focusPolicy();
    this->setLayout(assemble);
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
            mainWidget->select();
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
        if(object == moveButton){
            this->mainWidget->makeCharacterMove(this->mainWidget->selectedObjId, this->mainWidget->cursorCoord);
        }
        if(object == shootButton){
            this->mainWidget->makeCharacterShoot(this->mainWidget->selectedObjId, this->mainWidget->cursorCoord);
        }
        if(object == endTurnButton){
            this->mainWidget->endTurn();
        }
    }
    return false;
}
