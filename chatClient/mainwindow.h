#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBox>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QObject>
#include <QTreeView>
#include "talkbox.h"
#include "QListWidget"
namespace Ui {
class MainWindow;
}

class QStandardItemModel;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    TalkBox* myTalkBox;
    explicit MainWindow(int sockfd, int my_id, QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_model;
    int sockfd;
    int my_id;

private slots:
    void showMyTalkBox();
    void singleclicked(QListWidgetItem* item);
};


#endif // MAINWINDOW_H
