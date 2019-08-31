#include "login.h"
#include "ui_login.h"
#include <string>
#include "mainwindow.h"


char serverIp[20] = "10.195.15.196";
int serverPort = 8888;
char localIp[20];

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    sockfd = -1;
    ui->setupUi(this);
    connect(ui->btn_login,SIGNAL(clicked()),this,SLOT(loginTo()));
    connect(ui->btn_register,SIGNAL(clicked()),this,SLOT(registerTo()));
}

Login::~Login()
{
    delete ui;
}

void Login::loginTo()
{
    sockfd = cs.connectTo(serverIp,serverPort);//change server ip here
    if(-1 == sockfd){
        qDebug() << "sockfd = -1" << endl;
        return;
    }

    loginInfo lf;
    strcpy(lf.id ,ui->lineEdit_account->text().toStdString().c_str());
    strcpy(lf.pwd,ui->lineEdit_pwd->text().toStdString().c_str());
    //
    char loginInfoSendToServer[40] = {0};

    strcat(loginInfoSendToServer,"1|");
    strcat(loginInfoSendToServer,lf.id);
    strcat(loginInfoSendToServer,"|");
    strcat(loginInfoSendToServer,lf.pwd);
    qDebug() << loginInfoSendToServer << endl;

    if(!strcmp(lf.id,"") || !strcmp(lf.pwd,""))
    {
        qDebug() << "pwd or id is empty!" << endl;
        return;
    }



    ssize_t size = send(sockfd,(void*)&loginInfoSendToServer,sizeof(loginInfoSendToServer),0);
    if(-1 == size)
    {
        qDebug() << "send message error" << endl;
        return;
    }

    char flag;//server send client 1 means login successfully
    size = recv(sockfd,&flag,sizeof(flag),0);
    if('1' == flag)
    {
        //start interface
        qDebug() << "login successful" << endl;
        main_window = new MainWindow(sockfd, lf.id);
        main_window->show();
        this->close();

    }
    else if('0' == flag)
    {
        qDebug() << "login failed" << endl;
        return;
    }
    else{
        qDebug() << "other error" <<endl;
    }
}

void Login::registerTo()
{

    regist = new Register(sockfd);
    regist->show();
}



