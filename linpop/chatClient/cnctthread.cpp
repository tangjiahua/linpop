#include "cnctthread.h"
#include "QTime"
cnctThread::cnctThread(QObject *parent):
    sockfd(sockfd),QObject(parent)
{

}

cnctThread::~cnctThread()
{

}

void cnctThread::start()
{
    int ret = pthread_create(&thread,NULL,run,this);
    if(0!= ret)
        qDebug() << "create thread is fail" << endl;
}

void* cnctThread::run(void* arg)
{
    cnctThread* th = (cnctThread*)(arg);
\
    qDebug()<<"start connecting"<<endl;
    th->sockfd = (th->cs).connectTo(th->serverIp,th->serverPort);
    qDebug()<<"connect complete"<<endl;

    emit th->finished(th->sockfd);

}
