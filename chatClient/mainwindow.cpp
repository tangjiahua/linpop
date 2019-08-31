#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolbox.h"
#include <QToolBox>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <qdebug.h>
#include <QMetaType>
#include <QStandardItemModel>
#include <QObject>
//#include <QAbstractItemView>
#include <qevent.h>
#include "mulistitemdata.h"
#include "mulitemdelegate.h"
#include "QMessageBox"
#include "QCursor"
#include "string"
#include "connectserver.h"
QStandardItemModel *pModel = new QStandardItemModel();
//const QStringList icons = {
//    ":/QQ/1.jpg", ":/QQ/2.jpeg", ":/QQ/5.jpg",
//    ":/QQ/4.jpeg", ":/QQ/3.jpg", ":/QQ/6.jpeg",
//};
//const QStringList singers = {
//    "MC-Hotdog", "李荣浩", "Author",
//    "PACT", "杨千嬅", "张震岳",
//};

struct List{
    char account[20];
    char phaddr[2];
    char state[2] ;
    char group[2];
};
QStringList friendName;
QStringList photoAddress;
QStringList state;
QStringList group;

char friendNameCurrent[20] = {0};
char photoAddressCurrent[2] = {0};
char stateCurrent[2] = {0};
char groupCurrent[2] = {0};

//int photoAddressCurrent = -1;
//int stateCurrent = -1;
//int groupCurrent = -1;

MainWindow::MainWindow(int sockfd, char *my_id , QWidget *parent) :
    sockfd(sockfd),
    my_id(my_id),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    setGeometry(1500,0,266,675);
    ui->setupUi(this);
    dragListFromServer();
    setListView();


}


void MainWindow::dragListFromServer(){
    //int sockfd, int my_id
    char dragByName[40] = {0};
    strcat(dragByName, "2");
    strcat(dragByName, "|");
    strcat(dragByName, my_id);

    ssize_t size = send(sockfd,(void*)&dragByName,sizeof(dragByName),0);
    if(size == -1){
        qDebug()<< "send message error" << endl;
        return;
    }
    struct List ReceiveBuffer;

    while(size = recv(sockfd, &ReceiveBuffer, sizeof(ReceiveBuffer),0)){
        if(size < 0){
            qDebug()<< "send message error\n" << endl;
            return;
        }else if(size == 0){
            printf("Connection closed\n");
            return;
        }else{
                memset(friendNameCurrent, 0,sizeof(friendNameCurrent));
                memset(photoAddressCurrent, 0,sizeof(photoAddressCurrent));
                memset(groupCurrent, 0,sizeof(groupCurrent));
                memset(stateCurrent, 0,sizeof(stateCurrent));

                strcpy(friendNameCurrent, ReceiveBuffer.account);
                strcpy(photoAddressCurrent, ReceiveBuffer.phaddr);
                strcpy(groupCurrent, ReceiveBuffer.group);
                strcpy(stateCurrent, ReceiveBuffer.state);
                if(groupCurrent[0] == '\0'){
                    break;
                }
                photoAddress << (photoAddressCurrent);
                friendName.append(friendNameCurrent);
                //photoAddress.append(photoAddressCurrent);
                group.append(groupCurrent);
                state.append(stateCurrent);


        }
    }



}


void MainWindow::setListView(){
    QListWidgetItem *aItem; //每一行是一个QListWidgetItem
    //ui->listWidget->setViewMode(QListView::IconMode);
    ui->friendListWidget->clear(); //清除项
    int x = photoAddress.count();
    for(int i=0; i<x; i++)
    {
        QWidget *widget = new QWidget(ui->friendListWidget);
        //创建自定义的item
        widget->setStyleSheet("background:transparent;");
        QLabel *TypeLabel = new QLabel(widget);
        QString address = ":/QQ/";
        QString tmp = photoAddress.at(i);
        address.append(tmp);
        address.append(".jpg");
        QPixmap myPix(address);
        TypeLabel->setFixedSize(62, 62);
        TypeLabel->setPixmap(myPix);
        TypeLabel->setScaledContents(true);
        TypeLabel->setStyleSheet("QLabel{padding-left:5px;padding-right:10px;padding-top:5px;padding-bottom:10px}");
        QWidget *frontArea = new QWidget(widget);
        frontArea->setFixedHeight(45);
        QLabel *TextLabel = new QLabel(frontArea);

        TextLabel->setText(friendName.at(i));//设置好友名字

        QFont nameFont;
        nameFont.setFamily("微软雅黑");
        nameFont.setPointSize(11);
        TextLabel->setFont(nameFont);
        QLabel *TotalCopyLabel = new QLabel(frontArea);
        TotalCopyLabel->setText("Online");
        QVBoxLayout *verLayout = new QVBoxLayout;
        verLayout->setContentsMargins(0, 0, 0, 0);
        verLayout->setMargin(0);
        verLayout->setSpacing(0);
        verLayout->addWidget(TextLabel);
        verLayout->addWidget(TotalCopyLabel);
        frontArea->setLayout(verLayout);
        QHBoxLayout *horLayout = new QHBoxLayout;
        horLayout->setContentsMargins(0, 0, 0, 0);
        horLayout->setMargin(0);
        horLayout->setSpacing(0);
        horLayout->addWidget(TypeLabel);
        horLayout->addWidget(frontArea);
        widget->setLayout(horLayout);
        QListWidgetItem *ITEM = new QListWidgetItem();
        QSize size = ITEM->sizeHint();
        ITEM->setSizeHint(QSize(size.width(), 56));
        ui->friendListWidget->addItem(ITEM);
        widget->setSizeIncrement(size.width(), 90);
        ui->friendListWidget->setItemWidget(ITEM, widget);
    }
    QObject::connect(ui->friendListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleclicked(QListWidgetItem*)));

}



void MainWindow::showMyTalkBox(){
    myTalkBox = new TalkBox();
    myTalkBox->show();
    int x = ui->friendListWidget->currentRow();
    qDebug()<<x;
}



void MainWindow::singleclicked(QListWidgetItem*item)

{
    QMessageBox box(QMessageBox::Warning,"Choose","Choose What To Do");
        box.setWindowFlags(Qt::FramelessWindowHint);

        box.setStandardButtons(QMessageBox::Ok|QMessageBox::No|QMessageBox::Cancel|QMessageBox::Close);
        box.setButtonText(QMessageBox::Ok, QString("Talk"));
        box.setButtonText(QMessageBox::No, QString("Block"));
        box.setButtonText(QMessageBox::Cancel, QString("Delete"));

        QCursor *mouse;
        QPoint position;
        position.setX(mouse->pos().x()-140);
        position.setY(mouse->pos().y()-30);
        box.move(QPoint(position));

        int x = box.exec();
        switch (x) {
        case QMessageBox::Ok:
            showMyTalkBox();
            break;
        default:
            qDebug()<<"nothing";
            break;
        }
    return;
}









MainWindow::~MainWindow()
{
    delete ui;
}

