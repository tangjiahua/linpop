#include "login.h"
#include "ui_login.h"

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
    sockfd = cs.connectTo("10.10.10.10",8888);//change server ip here
    if(-1 == sockfd){
        qDebug() << "sockfd = -1" << endl;
        return;
    }

    loginInfo lf;
    strcpy(lf.id ,ui->lineEdit_account->text().toStdString().c_str());
    strcpy(lf.pwd,ui->lineEdit_pwd->text().toStdString().c_str());
    if(!strcmp(lf.id,"") || !strcmp(lf.pwd,""))
    {
        qDebug() << "pwd or id is empty!" << endl;
        return;
    }
    ssize_t size = send(sockfd,(void*)&lf,sizeof(lf),0);
    if(-1 == size)
    {
        qDebug() << "send message error" << endl;
        return;
    }

    int flag = 0;//server send client 1 means login successfully
    size = recv(sockfd,&flag,sizeof(int),0);
    if(1 == flag)
    {
        //start interface
        qDebug() << "login successful" << endl;
        main_face = new interface(sockfd,atoi(lf.id));
        main_face->show();
        this->close();
        delete this;
    }
    else
    {
        qDebug() << "server send me not 1!" << endl;
        return;
    }
}

void Login::registerTo()
{
    if(-1 == sockfd)
    {
        connectServer cs;
        sockfd = cs.connectTo("10.10.10.10",8888);
        if(-1 == sockfd)
        {
            qDebug() << "register don't connect server" << endl;
            return;
        }
    }
    regist = new Register(sockfd);
    regist->show();
}
