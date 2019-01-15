#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QSet>
#include <QKeyEvent>
#include "mainwidget.h"
#include <QProgressBar>
#include <QLabel>

struct CharacterInfoWidget{
    QProgressBar hpProgressBar;
    QLabel name;
    QColor color;
};

class Widget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    Widget(int randseed, int fps=60,unsigned int gridSize = 8, float size=0.5f, QWidget *parent = 0);
    bool eventFilter(QObject *object, QEvent *event) override;
    void timerEvent(QTimerEvent *e) override;
    MainWidget* mainWidget;

private:
    QBasicTimer timer;

    QPushButton *endTurnButton;
    QHBoxLayout *ui;

    vector<QVBoxLayout*> characterInfos;

    void displayCharacter();
};

#endif // WIDGET_H
