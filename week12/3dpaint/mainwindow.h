#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"objviewer.h"
#include <QMainWindow>
#include <QMdiArea>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void load();

private:
    Ui::MainWindow *ui;
    QMdiArea area;
    ObjViewer objviewer;
};

#endif // MAINWINDOW_H
