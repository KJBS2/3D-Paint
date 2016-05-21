#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include<string>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    area.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    area.addSubWindow(&objviewer);
    this->setCentralWidget(&area);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load()
{
    QFileDialog dialog(this);
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Load obj"),"C:\\",tr("Obj files (*.obj)"));
    if(fileName!="")this->objviewer.add(fileName);
}
