#ifndef CONNECTSERVER_H
#define CONNECTSERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <QDebug>

using namespace std;

class connectServer
{
public:
    connectServer();
    ~connectServer();

    int connectTo(string ip_addr,short port);
    static void closeSockfd(int sockfd);

private:
    struct sockaddr_in addr;//use to let network address transfer to Ubuntu 18.04 standarlized address
};

#endif // CONNECTSERVER_H
