#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBox>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QObject>

namespace Ui {
class MainWindow;
}

class QStandardItemModel;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int sockfd,int my_id,QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_model;
    int sockfd;
    int my_id;

private slots:

};


#endif // MAINWINDOW_H
