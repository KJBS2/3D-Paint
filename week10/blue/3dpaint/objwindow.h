#ifndef OBJWINDOW_H
#define OBJWINDOW_H

#include <QWidget>

class ObjWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ObjWindow(QWidget *parent = 0);
signals:

public slots:

private:
    //file directory
    //file information
    // ex) aVector aFace Camera.. etc
};

#endif // OBJWINDOW_H
