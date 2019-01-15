#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "ui_dialog.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
