#include"addfriend.h"
#include"ui_addfriend.h"
#include"QMessageBox"

addfriend::addfriend(int sockfd, char name[10], QWidget *parent) :
   sockfd(sockfd),QDialog(parent),
    ui(new Ui::addfriend)
{
    ui->setupUi(this);
    connect(ui->btn_add,SIGNAL(clicked()),this,SLOT(addTo()));
    connect(parent,SIGNAL(sendAddResult(char)),this,SLOT(receiveAddResult(char)));
    strcpy(username,name);
}

addfriend::~addfriend()
{
    delete ui;
}

void addfriend::addTo()
{
    ui->btn_add->setText("adding...");
    friendInfo fi;
    memset(&fi,0,sizeof(fi));

    if(ui->lineEdit_name->text()=="")
    {
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("add failed");//对话框标题
        msg.setText("Friend's name is empty! please try again");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Retry);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Retry)//模态调用
        {
           qDebug() << "Friend's name is empty!";//数据处理
           return;
        }
    }
    if(ui->lineEdit_invitecode->text()=="")
    {
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("add failed");//对话框标题
        msg.setText("Invite code is empty! please try again");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Retry);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Retry)//模态调用
        {
           qDebug() << "Invite code is empty!";//数据处理
           return;
        }
    }
    strcpy(invitecode,ui->lineEdit_invitecode->text().toStdString().c_str());
    if(strlen(invitecode)>7)
    {
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("add failed");//对话框标题
        msg.setText("Length can't exceed 7! please try again");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Retry);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Retry)//模态调用
        {
           qDebug() << "Length can't exceed 7!";//数据处理
           return;
        }
    }

    strcpy(fi.name,ui->lineEdit_name->text().toStdString().c_str());
    strcpy(fi.invitecode,ui->lineEdit_invitecode->text().toStdString().c_str());

    strcat(addInfoSendToServer,"4|");
    strcat(addInfoSendToServer,username);
    strcat(addInfoSendToServer,"|");
    strcat(addInfoSendToServer,fi.name);
    strcat(addInfoSendToServer,"|");
    strcat(addInfoSendToServer,fi.invitecode);

    qDebug() << addInfoSendToServer << endl;

    send(sockfd,&addInfoSendToServer,sizeof(addInfoSendToServer),0);

}

void addfriend::receiveAddResult(char result){
    ui->btn_add->setText("add");
    if(result=='1'){
        QMessageBox msg1(this);//对话框设置父组件
        msg1.setWindowTitle("add failed");//对话框标题
        msg1.setText("Target user does not exist!");//对话框提示文本
        msg1.setIcon(QMessageBox::Warning);//设置图标类型
        msg1.setStandardButtons(QMessageBox::Retry);//对话框上包含的按钮

        if(msg1.exec() == QMessageBox::Retry)//模态调用
        {
           qDebug() << "Target user does not exist!";//数据处理
           memset(addInfoSendToServer,0,sizeof(addInfoSendToServer));
           return;
        }
    }
    else if(result == '2'){
        QMessageBox msg2(this);//对话框设置父组件
        msg2.setWindowTitle("add failed");//对话框标题
        msg2.setText("You've been blocked by this user!");//对话框提示文本
        msg2.setIcon(QMessageBox::Warning);//设置图标类型
        msg2.setStandardButtons(QMessageBox::Retry);//对话框上包含的按钮

        if(msg2.exec() == QMessageBox::Retry)//模态调用
        {
           qDebug() << "You've been blocked by this user!";//数据处理
           memset(addInfoSendToServer,0,sizeof(addInfoSendToServer));
           return;
        }
    }
    else if(result=='3'){
        QMessageBox msg3(this);//对话框设置父组件
        msg3.setWindowTitle("add failed");//对话框标题
        msg3.setText("Wrong invitation code!");//对话框提示文本
        msg3.setIcon(QMessageBox::Warning);//设置图标类型
        msg3.setStandardButtons(QMessageBox::Retry);//对话框上包含的按钮

        if(msg3.exec() == QMessageBox::Retry)//模态调用
        {
           qDebug() << "Wrong invitation code!";//数据处理
           memset(addInfoSendToServer,0,sizeof(addInfoSendToServer));
           return;
        }
    }
    else if(result =='4'){
        QMessageBox msg4(this);//对话框设置父组件
        msg4.setWindowTitle("add succeeded");//对话框标题
        msg4.setText("add succeeded!");//对话框提示文本
        msg4.setIcon(QMessageBox::Information);//设置图标类型
        msg4.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮
        emit addSucceeded();
        if(msg4.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << "add succeed!";//数据处理
           memset(addInfoSendToServer,0,sizeof(addInfoSendToServer));
           return;
        }
    }
    else if(result=='5'){
        QMessageBox msg5(this);//对话框设置父组件
        msg5.setWindowTitle("add failed");//对话框标题
        msg5.setText("this user has already been in your list!");//对话框提示文本
        msg5.setIcon(QMessageBox::Warning);//设置图标类型
        msg5.setStandardButtons(QMessageBox::Retry);//对话框上包含的按钮

        if(msg5.exec() == QMessageBox::Retry)//模态调用
        {
           qDebug() << "this user has already been in your list!";//数据处理
           memset(addInfoSendToServer,0,sizeof(addInfoSendToServer));
           return;
        }
    }
    else{
        memset(addInfoSendToServer,0,sizeof(addInfoSendToServer));
        return;
    }

}
