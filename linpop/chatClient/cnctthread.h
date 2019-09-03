#ifndef CNCTTHREAD_H
#define CNCTTHREAD_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include <arpa/inet.h>
#include <string.h>
#include "connectserver.h"

class cnctThread : public QObject
{
    Q_OBJECT
public:
    cnctThread(QObject *parent = 0);
    ~cnctThread();

    void start();
    static void* run(void* arg);
signals:


    void disconnect_signal();
    void finished(int finishfd);
private:
    int sockfd;
    pthread_t thread;
    string serverIp = "10.194.46.38";
    short serverPort = 8888;
    char localIp[20];
    connectServer cs;
};
#endif // CNCTTHREAD_H
