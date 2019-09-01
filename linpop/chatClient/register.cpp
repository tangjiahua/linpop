#include "register.h"
#include "ui_register.h"
#include "QMessageBox"
#include "recvmsgthread.h"

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
    char serverIp[20] = "10.195.108.2";
    int serverPort = 8888;
    char localIp[20];
    char registInfo[50] = {0};

    ui->label_new_account->clear();

    registerInfo rf;
    char registInfoSendToServer[50] = {0};
    memset(&rf,0,sizeof(rf));
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
        return;
    }
    else if(!strcmp(rf.pwd,"")||!strcmp(cfmpwd,""))
    {
        ui->label_new_account->setText(QString("please set password"));
        return;
    }

    ui->btn_commit->setText("registering...");


    int rgfd = cs.connectTo(serverIp,serverPort);

    if(rgfd == -1){
        qDebug()<<"register connect error!"<<endl;
            ui->btn_commit->setText("register");
        return;
    }

    send(rgfd,&registInfoSendToServer,sizeof(registInfoSendToServer),0);

    recvMsgThread *rthread = new recvMsgThread(rgfd,this);
    rthread->start();

    connect(rthread,SIGNAL(sendMsg_signal_0()),this,SLOT(msg_received_0()));
    connect(rthread,SIGNAL(sendMsg_signal_1()),this,SLOT(msg_received_1()));
    connect(rthread,SIGNAL(sendMsg_signal_2()),this,SLOT(msg_received_2()));
    connect(rthread,SIGNAL(internetdisconnect_signal()),this,SLOT(internetdisconnected_received()));


}

void Register::msg_received_0(){
    ui->btn_commit->setText("register");
    QMessageBox msg(this);//对话框设置父组件
    msg.setWindowTitle("error");//对话框标题
    msg.setText("UNKNOW ERROR!");//对话框提示文本
    msg.setIcon(QMessageBox::Critical);//设置图标类型
    msg.setStandardButtons(QMessageBox::Cancel);//对话框上包含的按钮

    if(msg.exec() == QMessageBox::Cancel)//模态调用
    {
       qDebug() << "unknown error";//数据处理
       return;
    }
}

void Register::msg_received_1(){
    ui->btn_commit->setText("register");
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

void Register::msg_received_2(){
    ui->btn_commit->setText("register");
    QMessageBox msg(this);//对话框设置父组件
    msg.setWindowTitle("register failed");//对话框标题
    msg.setText("this username has been registered");//对话框提示文本
    msg.setIcon(QMessageBox::Information);//设置图标类型
    msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

    if(msg.exec() == QMessageBox::Ok)//模态调用
    {
       qDebug() << "regist failed";//数据处理
       return;
    }
}

void Register::internetdisconnected_received(){
    ui->btn_commit->setText("register");
    QMessageBox msg(this);//对话框设置父组件
    msg.setWindowTitle("error");//对话框标题
    msg.setText("internet disconncted, please check your internet settings and try again.");//对话框提示文本
    msg.setIcon(QMessageBox::Critical);//设置图标类型
    msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

    if(msg.exec() == QMessageBox::Ok)//模态调用
    {
       qDebug() << "internet disconnected";//数据处理
       return;
    }
}


