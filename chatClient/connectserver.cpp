#include "connectserver.h"

connectServer::connectServer()
{

}

connectServer::~connectServer()
{

}

int connectServer::connectTo(string ip_addr, short port)
{
    //init the socket addr
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

    //Socket()
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sockfd)
    {
        qDebug() << "create sockfd error\n" << endl;
        return -1;
    }

    //Connect
    int res = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(-1 == res)
    {
        qDebug() << "connect error\n" << endl;
        close(sockfd);
        return -1;
    }
    qDebug() << "connect successful!" << endl;
    return sockfd;
}

void connectServer::closeSockfd(int sockfd)
{
    close(sockfd);
}
