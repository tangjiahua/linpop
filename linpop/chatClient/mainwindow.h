#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBox>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QObject>
//#include <QTreeView>
#include "talkbox.h"
#include "QListWidget"
#include "QTime"
namespace Ui {
class MainWindow;
}

class QStandardItemModel;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    TalkBox* myTalkBox[100] = {nullptr};
    explicit MainWindow(int sockfd, char *my_id, QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_model;

    int sockfd;
    char *my_id;

private slots:
    void showMyTalkBox(QString talkTo, QString photoAddr, QString signature,QStringList record);
    void singleclicked(QListWidgetItem* item);
    void dragMyInformation();
    void dragListFromServer();
    void setListView();
    void closeEvent(QCloseEvent *event);
};


#endif // MAINWINDOW_H
