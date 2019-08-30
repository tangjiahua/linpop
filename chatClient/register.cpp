#include "register.h"
#include "ui_register.h"
#include "QMessageBox"


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
    char serverIp[20] = "10.194.41.208";
    int serverPort = 8888;
    char localIp[20];
    char registInfo[50] = {0};
    registerInfo rf;
    char registInfoSendToServer[50] = {0};
    memset(&rf,0,sizeof(rf));
    if(ui->lineEdit_name->text()==""||ui->lineEdit_pwd->text()==""){
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

    int rgfd = cs.connectTo(serverIp,serverPort);

    if(rgfd == -1){
        qDebug()<<"register connect error!"<<endl;
        return;
    }

    strcat(registInfoSendToServer,"0|");
    strcat(registInfoSendToServer,rf.name);
    strcat(registInfoSendToServer,"|");
    strcat(registInfoSendToServer,rf.pwd);
    qDebug() << registInfoSendToServer << endl;

    if(!strcmp(rf.name,"") || !strcmp(rf.pwd,""))
    {
        qDebug() << "pwd or id is empty!" << endl;
        return;
    }

    send(rgfd,&registInfoSendToServer,sizeof(registInfoSendToServer),0);


    char is_registered = '-1';//status of the account if it has been registered before?


    recv(rgfd,&is_registered,sizeof(char),0);



    if(is_registered == '1'){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("congratrulations");//对话框标题
        msg.setText("registed successfully, you can go to login now!");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
            this->close();
           qDebug() << "regist succeeded";//数据处理
        }
//        ui->label_new_account->setText(QString("registed successfully, you can go to login now!"));
        qDebug()<<"you can go to login now"<<endl;
    }
    else if(is_registered == '0'){
        QMessageBox msg(this);//对话框设置父组件
        msg.setWindowTitle("information");//对话框标题
        msg.setText("accout already been registered before by someone");//对话框提示文本
        msg.setIcon(QMessageBox::Information);//设置图标类型
        msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮

        if(msg.exec() == QMessageBox::Ok)//模态调用
        {
           qDebug() << "regiset failed";//数据处理
           return;
        }
    }

}


