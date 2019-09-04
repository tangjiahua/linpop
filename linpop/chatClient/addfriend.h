#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>
#include <sys/types.h>
#include <sys/socket.h>
#include "connectserver.h"

namespace Ui {
class addfriend;
}

struct friendInfo
{
  char name[20]={0};
  char invitecode[20]={0};
};

class addfriend : public QDialog
{
    Q_OBJECT

public:
    explicit addfriend(int sockfd, char username[10], QWidget *parent = 0);
    ~addfriend();

private:
    Ui::addfriend *ui;
    int sockfd;
    char username[10];
    char invitecode[10];
    connectServer cs;
    char addInfoSendToServer[50] = {0};

private slots:
    void addTo();
    void receiveAddResult(char result);

signals:
    void addSucceeded();

};

#endif // ADDFRIEND_H
