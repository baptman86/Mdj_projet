#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QSet>
#include <QKeyEvent>
#include "mainwidget.h"

class Widget : public QOpenGLWidget
{
public:
    Widget(int randseed, int fps=60,unsigned int gridSize = 8, float size=0.5f, QWidget *parent = 0);
    bool eventFilter(QObject *object, QEvent *event) override;
    MainWidget* mainWidget;

private:
    QPushButton *moveButton;
    QPushButton *shootButton;
    QPushButton *endTurnButton;
};

#endif // WIDGET_H
