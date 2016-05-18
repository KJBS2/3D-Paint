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
    void add();
private:
//    ObjWindow windows; there is an error
    QTabWidget t;
};

#endif // OBJVIEWER_H
