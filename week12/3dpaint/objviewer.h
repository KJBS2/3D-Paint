#ifndef OBJVIEWER_H
#define OBJVIEWER_H


#include"objwindow.h"
#include<QMdiSubWindow>
#include<QTabWidget>


#define MAXI 10

class ObjViewer : public QMdiSubWindow
{
public:
    ObjViewer();
    void add(QString);
private:

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *key);
    ObjWindow* aWindow[MAXI];
    QTabWidget tab;
    int nWindow;
};

#endif // OBJVIEWER_H
