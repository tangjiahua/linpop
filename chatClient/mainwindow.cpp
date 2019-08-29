#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(int sockfd, int my_id , QWidget *parent) :
    sockfd(sockfd),
    my_id(my_id),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
