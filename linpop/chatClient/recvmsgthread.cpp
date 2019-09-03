#include "recvmsgthread.h"
#include "QTime"
recvMsgThread::recvMsgThread(int sockfd, QObject *parent):
    sockfd(sockfd),QObject(parent)
{

}

recvMsgThread::~recvMsgThread()
{
    qDebug()<<"thread die"<<endl;
}

void recvMsgThread::start()
{
    int ret = pthread_create(&thread,NULL,run,this);
    if(0!= ret)
        qDebug() << "create thread is fail" << endl;
}

void* recvMsgThread::run(void* arg)
{
    recvMsgThread* th = (recvMsgThread*)(arg);
    char message;
    QTime t;
    t.start();
    while(t.elapsed()<5000)
    {
       int ret = recv(th->sockfd,&message,sizeof(message),0);
       if(-1 == ret || 0 == ret)
            break;
       else{
           emit th->sendMsg_signal(message);
           qDebug() << "recvMsgthread::recv a message:" << message << endl;
           break;
       }
    }

    if(t.elapsed()>=5000){
            emit th->timeout_signal();
        qDebug()<<"timeout"<<endl;
        th->~recvMsgThread();
        return nullptr;
    }
    qDebug()<<"recvMsgthread end"<<endl;
    th->~recvMsgThread();
    return nullptr;
}
