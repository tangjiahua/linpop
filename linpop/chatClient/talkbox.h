#ifndef TALKBOX_H
#define TALKBOX_H

#include <QMainWindow>
#include "connectserver.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <map>
#include <QCloseEvent>
namespace Ui {
class TalkBox;
}

class TalkBox : public QMainWindow
{
    Q_OBJECT

public:
    int sockfd;
    QString myAccount;
    QString addrofname;
    QWidget *parent;
    map<QString,TalkBox*> *mp;
    explicit TalkBox(int sockfd, QString myAccount, QString addrofname, map<QString,TalkBox*> *mp,QWidget *parent);
    ~TalkBox();
    QStringList talkboxrecord;
    int recordno;
//    void* recvThread(void *args);

private slots:

    void on_pushButton_clicked();

    void on_sendButton_clicked();

    void receive_message(char *uName,char *fName,char *sendDate,char* sendMessage);

    void closeEvent(QCloseEvent *event);

private:
    Ui::TalkBox *ui;
    pthread_t thread;
};

#endif // TALKBOX_H
