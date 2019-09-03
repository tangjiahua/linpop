#include "talkbox.h"
#include "ui_talkbox.h"
#include "qscrollbar.h"
#include "qmessagebox.h"
#include "mainwindow.h"
#include "QTime"


//int gSockfd;
//QString gmyAccount;
//QString gaddrofname;
//QString gaddrofpicture;
//QString gsignature;
//QStringList grecord;

void* recvThread(void *args);

TalkBox::TalkBox(int tsockfd, QString tmyAccount, QString taddrofname,QWidget *tparent) :
    QMainWindow(parent),
    ui(new Ui::TalkBox)
{
    sockfd = tsockfd;
    myAccount = tmyAccount;
    addrofname = taddrofname;
    parent = tparent;
    //gaddrofpicture = addrofpicture;
    //gsignature = signature;


    //设置聊天窗口的各种讯息
    setGeometry(600,100,266,675);
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_sendButton_clicked()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
    connect(parent,SIGNAL(receiveChatMsg(char*,char*,char*,char*)),this,SLOT(receive_message(char*,char*,char*,char*)));

//    QString phoAdd;
//    phoAdd.append(":/QQ/");
//    phoAdd.append(addrofpicture);
//    phoAdd.append(".jpg");
//    QPixmap p;
//    p.load(phoAdd);
//    ui->talkerpicture->setPixmap(p);
//    ui->talkerpicture->setScaledContents(true);
    ui->talkername->setText(addrofname);
//    ui->talkersignature->setText(signature);

    //TODO::设置抓取往常的聊天记录
//    talkboxrecord=record;
//    recordno=talkboxrecord.count()-1;
//    ui->textEdit->clear();


    //TODO::每次打开这个要先从服务器抓取十条原来的记录

//    int ret;
    //TODO::开始新线程recv;
//    pthread_t thread;
//    ret = pthread_create(&thread, NULL, recvThread, NULL);
//    if(0 != ret){
//        qDebug()<<"create thread is fail"<<endl;
//    }




    //TODO::一接受到就打印在上面


}

TalkBox::~TalkBox()
{
    delete ui;
}

void* recvThread(void *args){
    while(1){

    }
}

void TalkBox::on_pushButton_clicked()
{
    if(recordno<0)
    {
        QMessageBox::warning(this,tr("warning"),tr("No more historical messages."),QMessageBox::Yes);
        return;
    }
    QTextCursor talkboxcursor=ui->textEdit->textCursor();

    talkboxcursor.movePosition((QTextCursor::Start));
    ui->textEdit->textChanged();
    talkboxcursor.insertText("\n");

    talkboxcursor.movePosition((QTextCursor::Start));

    for(int j=1;recordno>=0 && j<=10;recordno--,j++)
    {
        talkboxcursor.insertText(talkboxrecord[recordno]);
        talkboxcursor.movePosition((QTextCursor::StartOfLine));
        if(j!=10 && recordno!=0)
        {
            talkboxcursor.insertText("\n");
        }

        talkboxcursor.movePosition((QTextCursor::Start));
    }




}

void TalkBox::on_sendButton_clicked()
{
    QTime t = QTime::currentTime();
        QString text = t.toString("hh:mm:ss");
        struct Msg{
            char *uName;
            char *fName;
            char *sendDate;
            char *sendMessage;
        };

        struct Msg msg;
//        memset(msg.uName, 0, sizeof(msg.uName));
//        memset(msg.fName, 0, sizeof(msg.fName));
//        memset(msg.sendDate, 0, sizeof(msg.sendDate));
//        memset(msg.sendMessage, 0, sizeof(msg.sendMessage));

        //首先把lineEdit内容发送给服务器
        QString message = ui->textEdit->toPlainText();
        QByteArray tmp1;
        tmp1 = myAccount.toLatin1();
        msg.uName = tmp1.data();

        QByteArray tmp2;
        tmp2 = addrofname.toLatin1();
        msg.fName = tmp2.data();

        QByteArray tmp3;
        tmp3 = ui->lineEdit->toPlainText().toLatin1();
        msg.sendMessage = tmp3.data();
        tmp3.clear();

        QByteArray tmp4;
        tmp4 = text.toLatin1();
        msg.sendDate = tmp4.data();
        tmp4.clear();



        //qDebug()<<msg.uName<<msg.fName<<msg.sendDate<<msg.sendMessage<<endl;
        char msgBox[300];
        memset(msgBox, 0, sizeof(msgBox));
        strcat(msgBox, "7|");
        strcat(msgBox, msg.uName);
        strcat(msgBox, "|");
        strcat(msgBox, msg.fName);
        strcat(msgBox, "|");
        strcat(msgBox, msg.sendDate);
        strcat(msgBox, "|");
        strcat(msgBox, msg.sendMessage);
        send(sockfd, &msgBox, sizeof(msgBox), 0);




        //把本地的lineEdit内容发送到框上
        //QTextCursor talkboxcursor = ui->textEdit->textCursor();
        ui->textEdit->append(ui->lineEdit->toPlainText());
        ui->lineEdit->clear();
        QScrollBar *scrollbar = ui->textEdit->verticalScrollBar();
        scrollbar->setSliderPosition(scrollbar->maximum());
        return;
}

void TalkBox::receive_message(char *uName, char *fName, char *sendDate, char *sendMessage){

    qDebug()<<"uName "<<uName<<endl;
    qDebug()<<"fName "<<fName<<endl;
    qDebug()<<"Date "<<sendDate<<endl;
    qDebug()<<"Message "<<sendMessage<<endl;
    char nameAndDate[30]={0};
    strcat(nameAndDate,uName);
    strcat(nameAndDate," ");
    strcat(nameAndDate,sendDate);
    strcat(nameAndDate,"\n");
    strcat(sendMessage,"\n");

//    qDebug()<<nameAndDate;
//    qDebug()<<sendMessage;
    ui->textEdit->append(QString(QLatin1String(nameAndDate)));
    ui->textEdit->append(QString(QLatin1String(sendMessage)));
    ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());
}


