#ifndef RECVTHREAD_H
#define RECVTHREAD_H
#include "msg.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

class recvThread : public QObject
{
    Q_OBJECT
public:
    recvThread(int sockfd,QObject *parent = 0);
    ~recvThread();

    void start();
    static void* run(void* arg);
    bool mainwindowclosed = false;
signals:
    void sendMsg_signal(char *message);
    void unknownerror();
    void signal3(char *message);
    void signal2(char *message);
    void signal5(char *message);
    void signal7(char *message);
    void signalx(char *message);
    void signala(char *message);
    void finished();
private:
    int sockfd;
    pthread_t thread;

};

#endif // RECVTHREAD_H
