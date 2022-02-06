#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actiondrawRect_triggered()
{
    ui->openGLWidget->drawShape(ABXOpenglWidget::Shape::eRect);
}


void MainWindow::on_actionclear_triggered()
{
    ui->openGLWidget->drawShape(ABXOpenglWidget::Shape::eNone);
}


void MainWindow::on_actionPolygonMode_triggered()
{
    ui->openGLWidget->setWireMode(ui->actionPolygonMode->isChecked());
}

