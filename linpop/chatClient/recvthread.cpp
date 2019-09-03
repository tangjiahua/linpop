#include "recvthread.h"
#include "QTime"
recvThread::recvThread(int sockfd, QObject *parent):
    sockfd(sockfd),QObject(parent)
{

}

recvThread::~recvThread()
{

}

void recvThread::start()
{
    int ret = pthread_create(&thread,NULL,run,this);
    if(0!= ret)
        qDebug() << "create thread is fail" << endl;
}

void* recvThread::run(void* arg)
{
    recvThread* th = (recvThread*)(arg);

    char message[300]  = {0};
    char messageCopy[300] = {0};
    while(1)
    {
        //memset(&msg,0,sizeof(msg));
        memset(message,0,sizeof(message));
        qDebug()<<"start receiving"<<endl;


        int ret = recv(th->sockfd,message,sizeof(message),0);

        if(-1 == ret || 0 == ret){
            qDebug()<<"unknown error when recv message ret!!!!!!!"<<endl;
            emit th->unknownerror();
        }
        memset(messageCopy, 0, sizeof(messageCopy));
        strcat(messageCopy, message);

        qDebug() << "recv a message: " << message << endl;
        char head = message[0];
        qDebug()<<"head == "<<head<<endl;
        switch(head){
            case '1':break;
        case '2':emit th->signal2(messageCopy);break;
        case '3':
            qDebug()<<"message == "<<messageCopy<<endl;
            emit th->signal3(messageCopy);

            break;
        case '4':break;
            case '5':break;
            case '6':break;
            case '7':
            qDebug()<<"message == "<<messageCopy<<endl;


            emit th->signal7(messageCopy);

            break;
            case '8':break;
            case 'a':break;
            case 'b':break;
            case 'c':break;
            case 'd':break;
        case 'x':emit th->signalx(messageCopy);
            break;
        }
//        emit th->sendMsg_signal(message);

    }
    qDebug()<<th->mainwindowclosed<<endl;
    emit th->finished();
    return nullptr;
}
