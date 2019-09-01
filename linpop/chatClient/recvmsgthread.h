#ifndef RECVMSGTHREAD_H
#define RECVMSGTHREAD_H
#include "msg.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

class recvMsgThread : public QObject
{
    Q_OBJECT
public:
    recvMsgThread(int sockfd,QObject *parent = 0);
    ~recvMsgThread();

    void start();
    static void* run(void* arg);
signals:

    void sendMsg_signal_0();
    void sendMsg_signal_1();
    void sendMsg_signal_2();
    void internetdisconnect_signal();
private:
    int sockfd;
    pthread_t thread;
};

#endif // RECVMSGTHREAD_H
