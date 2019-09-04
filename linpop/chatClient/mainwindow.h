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
#include "recvthread.h"
#include <map>

namespace Ui {
class MainWindow;
}

//struct Information{
//    char account[20];
//    char phaddr[2];
//    char autograph[30];
//    char state[2];
//};

class QStandardItemModel;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
//    TalkBox* myTalkBox[100] = {nullptr};
    explicit MainWindow(int sockfd, char *my_id, QWidget *parent = 0);
    map<QString,TalkBox*> mp;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_model;

    int sockfd;
    char *my_id;

    char dragMyInformationByName[40] = {0};

    recvThread *rthread;

private slots:
    void showMyTalkBox(int sockfd, QString myAccount, QString talkTo);
    void singleclicked(QListWidgetItem* item);
    void dragMyInformation();
    void dragListFromServer();
    void setListView();
    void closeEvent(QCloseEvent *event);
    void receive_msg(char *);
    void receive3(char *message);
    void receive2(char *message);
    void receive7(char *message);

    void on_refreshButton_clicked();

signals:
    void close_signal();
    void receiveChatMsg(char *uName,char *fName,char *snedDate,char *sendMessage);
};


#endif // MAINWINDOW_H
