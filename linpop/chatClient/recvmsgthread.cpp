#include "recvmsgthread.h"
#include "QTime"
recvMsgThread::recvMsgThread(int sockfd, QObject *parent):
    sockfd(sockfd),QObject(parent)
{

}

recvMsgThread::~recvMsgThread()
{

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
    char message[50]  ={0};
    QTime t;
    t.start();
    while(t.elapsed()<5000)
    {
        //memset(&msg,0,sizeof(msg));
        int ret = recv(th->sockfd,message,sizeof(message),0);
        if(-1 == ret || 0 == ret)
            break;
        if(!strcmp(message, "1")){
                emit th->sendMsg_signal_1();
                break;
        }else if(!strcmp(message, "2")){
                emit th->sendMsg_signal_2();
                break;
        }else{
            emit th->sendMsg_signal_0();
            break;
        }
//       qDebug() << "recv a message: " << message << endl;
    }
    if(t.elapsed()>=5000){
            emit th->internetdisconnect_signal();
    }
}
