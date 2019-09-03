#include "register.h"
#include "ui_register.h"
#include "QMessageBox"
#include "recvmsgthread.h"
#include "cnctthread.h"

Register::Register(int sockfd, QWidget *parent) :
   sockfd(sockfd),QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    connect(ui->btn_commit,SIGNAL(clicked()),this,SLOT(commitTo()));
}

Register::~Register()
{
    delete ui;
}

void Register::commitTo()
{

    ui->label_new_account->clear();

    registerInfo rf;

    memset(&rf,0,sizeof(rf));
    memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
    if(ui->lineEdit_name->text()==""||ui->lineEdit_pwd->text()==""){
        ui->label_new_account->setText(QString("account or password wasn't been filt"));
        qDebug() << "account or password wasn't been filt" << endl;
        return;
    }

    strcpy(rf.name,ui->lineEdit_name->text().toStdString().c_str());
    strcpy(rf.pwd,ui->lineEdit_pwd->text().toStdString().c_str());

    char cfmpwd[20] = {0};

    strcpy(cfmpwd,ui->lineEdit_cfmpwd->text().toStdString().c_str());

    if(strcmp(rf.pwd,cfmpwd)){
        ui->label_new_account->setText(QString("passwords don't match!"));
        memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
        return;
    }

    strcat(registInfoSendToServer,"0|");
    strcat(registInfoSendToServer,rf.name);
    strcat(registInfoSendToServer,"|");
    strcat(registInfoSendToServer,rf.pwd);
    qDebug() << registInfoSendToServer << endl;

    if(!strcmp(rf.name,""))
    {
        ui->label_new_account->setText(QString("username is empty, please fill in username"));
        memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
        return;
    }
    else if(!strcmp(rf.pwd,"")||!strcmp(cfmpwd,""))
    {
        ui->label_new_account->setText(QString("please set password"));
        memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
        return;
    }

    ui->btn_commit->setText("registering...");

    cnctThread *cthread = new cnctThread(this);
    cthread->start();

    connect(cthread,SIGNAL(finished(int)),this,SLOT(startrecv(int )));




}



void Register::msg_received(char *message){
    ui->btn_commit->setText("commit");
    if(!strcmp(message,"1")){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("registed successfully");//对话框标题
        msg.setText("you can go to login now!");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
            qDebug() << "regist succeeded";//数据处理
            this->close();
        }
    }
    else if(!strcmp(message,"2")){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("register failed");//对话框标题
        msg.setText("this username has been registered");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << "regist failed";//数据处理
           memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
           return;
        }
    }
    else if(!strcmp(message,"0")){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("error");//对话框标题
        msg.setText("UNKNOW ERROR!");//对话框提示文本
        msg.setIcon(QMessageBox::Critical);//设置图标类型
        msg.setStandardButtons(QMessageBox::Cancel);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Cancel)//模态调用
        {
           qDebug() << "unknown error";//数据处理
           memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
           return;
        }
    }
}

void Register::timeout_received(){
    ui->btn_commit->setText("register");
    QMessageBox msg(this);//对话框设置父组件
    msg.setWindowTitle("error");//对话框标题
    msg.setText("internet disconncted, please check your internet settings and try again.");//对话框提示文本
    msg.setIcon(QMessageBox::Critical);//设置图标类型
    msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

    if(msg.exec() == QMessageBox::Ok)//模态调用
    {
       qDebug() << "internet disconnected";//数据处理
       memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
       return;
    }
}

void Register::startrecv(int t_sockfd){
    if(t_sockfd==-1){
        ui->btn_commit->setText("commit");
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("error");//对话框标题
        msg.setText("cannot connect to server.");//对话框提示文本
        msg.setIcon(QMessageBox::Critical);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << "cannot connect to server";//数据处理
           memset(registInfoSendToServer,0,sizeof(registInfoSendToServer));
           return;
        }
    }
    sockfd = t_sockfd;

    send(sockfd,&registInfoSendToServer,sizeof(registInfoSendToServer),0);

    recvMsgThread *rthread = new recvMsgThread(sockfd,this);
    rthread->start();

    connect(rthread,SIGNAL(sendMsg_signal(char *)),this,SLOT(msg_received(char *)));

    connect(rthread,SIGNAL(timeout_signal()),this,SLOT(timeout_received()));

}


