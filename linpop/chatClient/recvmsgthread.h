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

    void timeout_signal();
    void sendMsg_signal(char  message);
private:
    int sockfd;
    pthread_t thread;
};

#endif // RECVMSGTHREAD_H
