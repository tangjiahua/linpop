#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <sys/types.h>
#include <sys/socket.h>
#include "connectserver.h"

namespace Ui {
class Register;
}

struct registerInfo
{
  char name[20]={0};
  char pwd[20]={0};
};

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(int sockfd,QWidget *parent = 0);
    ~Register();

private:
    Ui::Register *ui;
    int sockfd;
    connectServer cs;

private slots:
    void commitTo();
};

#endif // REGISTER_H
