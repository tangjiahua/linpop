#include "login.h"
#include "ui_login.h"
#include <string>
#include <QMessageBox>
#include "mainwindow.h"
#include "recvmsgthread.h"
#include "cnctthread.h"



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
        memset(loginInfoSendToServer,0,sizeof(loginInfoSendToServer));
    memset(lf.id,0,sizeof(lf.id));
    memset(lf.pwd,0,sizeof(lf.pwd));
    strcpy(lf.id ,ui->lineEdit_account->text().toStdString().c_str());
    strcpy(lf.pwd,ui->lineEdit_pwd->text().toStdString().c_str());


    strcat(loginInfoSendToServer,"1|");
    strcat(loginInfoSendToServer,lf.id);
    strcat(loginInfoSendToServer,"|");
    strcat(loginInfoSendToServer,lf.pwd);
    qDebug() << loginInfoSendToServer << endl;

    if(!strcmp(lf.id,"") || !strcmp(lf.pwd,""))
    {
        qDebug() << "pwd or id is empty!" << endl;
        ui->btn_login->setText("login");
            memset(loginInfoSendToServer,0,sizeof(loginInfoSendToServer));
        return;
    }

    ui->btn_login->setText("loging...");


    cnctThread *cthread = new cnctThread(this);
    cthread->start();

    connect(cthread,SIGNAL(finished(int)),this,SLOT(startrecv(int)));

}

void Login::registerTo()
{
    regist = new Register(sockfd);
    regist->show();
}

void Login::msg_received(char  message){
    ui->btn_login->setText("login");
    qDebug()<<"MESSAGE == "<<message<<endl;
    if(message=='1'){
        qDebug() << "login successful" << endl;


        main_window = new MainWindow(sockfd, lf.id);
        main_window->show();
        this->close();
    }
    else if(message == '2'){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("login failed");//对话框标题
        msg.setText("wrong username or password, please try again");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << "login failed";//数据处理
               memset(loginInfoSendToServer,0,sizeof(loginInfoSendToServer));
           return;
        }
    }
    else if(message == '0'){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("error");//对话框标题
        msg.setText("UNKNOW ERROR!");//对话框提示文本
        msg.setIcon(QMessageBox::Critical);//设置图标类型
        msg.setStandardButtons(QMessageBox::Cancel);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Cancel)//模态调用
        {
           qDebug() << "unknown error";//数据处理
               memset(loginInfoSendToServer,0,sizeof(loginInfoSendToServer));
           return;
        }
    }
}

void Login::startrecv(int t_sockfd){
    qDebug()<<"start receiving"<<endl;
    if(t_sockfd==-1){
        ui->btn_login->setText("login");
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("error");//对话框标题
        msg.setText("cannot connect to server.");//对话框提示文本
        msg.setIcon(QMessageBox::Critical);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << "cannot connect to server";//数据处理
               memset(loginInfoSendToServer,0,sizeof(loginInfoSendToServer));
           return;
        }
    }

    sockfd = t_sockfd;
    qDebug()<<"start sending"<<endl;
    ssize_t size = send(sockfd,(void*)&loginInfoSendToServer,sizeof(loginInfoSendToServer),0);
    if(-1 == size)
    {
        qDebug() << "send message error" << endl;
        ui->btn_login->setText("login");
            memset(loginInfoSendToServer,0,sizeof(loginInfoSendToServer));
        return;
    }

    recvMsgThread *rthread = new recvMsgThread(sockfd,this);
    rthread->start();

    connect(rthread,SIGNAL(sendMsg_signal(char )),this,SLOT(msg_received(char )));
    connect(rthread,SIGNAL(timeout_signal()),this,SLOT(timeout_received()));

}

void Login::timeout_received(){
    ui->btn_login->setText("login");
    QMessageBox msg(this);//对话框设置父组件
    msg.setWindowTitle("error");//对话框标题
    msg.setText("internet disconncted, please check your internet settings and try again.");//对话框提示文本
    msg.setIcon(QMessageBox::Critical);//设置图标类型
    msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

    if(msg.exec() == QMessageBox::Ok)//模态调用
    {
       qDebug() << "internet disconnected";//数据处理
           memset(loginInfoSendToServer,0,sizeof(loginInfoSendToServer));
       return;
    }
}



