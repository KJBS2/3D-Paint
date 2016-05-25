#include "objviewer.h"
#include<QPushButton>
#include<QString>
#include<QStringRef>

#define VIEWER_WIDTH 1300
#define VIEWER_HEIGHT 680
ObjViewer::ObjViewer()
{
    this->setFixedSize(VIEWER_WIDTH,VIEWER_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWidget(&tab);
    nWindow=0;
}

void ObjViewer::add(QString fileName)
{
    aWindow[nWindow]=new ObjWindow(fileName);
    int idx=0;
    for(int i=0;i<fileName.length();++i) if(fileName.at(i)=='/') idx=i;
    tab.addTab(aWindow[nWindow],fileName.mid(idx+1,fileName.length()-idx));
    tab.setCurrentWidget(aWindow[nWindow]);
    ++nWindow;
}
void ObjViewer::keyPressEvent(QKeyEvent *key)
{
    ((ObjWindow*)tab.currentWidget())->inputKey(key);
}
void ObjViewer::keyReleaseEvent(QKeyEvent *key)
{
    ((ObjWindow*)tab.currentWidget())->releaseKey(key);
}
