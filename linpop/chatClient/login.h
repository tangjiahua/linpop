#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "connectserver.h"
#include "register.h"
#include "mainwindow.h"


namespace Ui {
class Login;
}

struct loginInfo
{
  char id[20]={0};
  char pwd[20]={0};
};

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private:
    Ui::Login *ui;

    MainWindow* main_window;
    Register* regist;
    int sockfd;
    connectServer cs;
    loginInfo lf;
private slots:
    void loginTo();
    void registerTo();
    void msg_received_0();
    void msg_received_1();
    void msg_received_2();
    void internetdisconnected_received();
};

#endif // LOGIN_H
