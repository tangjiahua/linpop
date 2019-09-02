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
namespace Ui {
class TalkBox;
}

class TalkBox : public QMainWindow
{
    Q_OBJECT

public:
    explicit TalkBox(int sockfd, QString myAccount, QString addrofname, QString addrofpicture,QString signature,QStringList record,QWidget *parent = 0);
    ~TalkBox();
    QStringList talkboxrecord;
    int recordno;
//    void* recvThread(void *args);

private slots:

    void on_pushButton_clicked();

    void on_sendButton_clicked();




private:
    Ui::TalkBox *ui;
    pthread_t thread;
};

#endif // TALKBOX_H
