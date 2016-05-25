#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
}
