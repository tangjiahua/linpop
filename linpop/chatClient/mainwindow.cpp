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
#include "QWidget"
#include <QApplication>

QStandardItemModel *pModel = new QStandardItemModel();

struct TalkerInfo{
    char talkerPhotoAddr[2];
    char talkeSig[50];
};

struct List{
    char account[20];
    char phaddr[2];
    char state[2] ;
    char group[2];
};

struct Information{
    char account[20];
    char phaddr[2];
    char autograph[30];
    char state[2];
};
char tmpInformattion[2];




QStringList friendName;
QStringList photoAddress;
QStringList state;
QStringList group;

char friendNameCurrent[20] = {0};
char photoAddressCurrent[2] = {0};
char stateCurrent[2] = {0};
char groupCurrent[2] = {0};



MainWindow::MainWindow(int sockfd, char *my_id , QWidget *parent) :
    sockfd(sockfd),
    my_id(my_id),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    setGeometry(1500,0,266,675);
    ui->setupUi(this);

//    QTime current_time = QTime::currentTime();
//    QDateTime current_date_time =QDateTime::currentDateTime();
//    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
//    qDebug()<<current_date<<endl;

    dragMyInformation();
    dragListFromServer();
    setListView();
}

void MainWindow::dragMyInformation(){
    char dragMyInformationByName[40] = {0};
    strcat(dragMyInformationByName, "3");
    strcat(dragMyInformationByName, "|");
    strcat(dragMyInformationByName, my_id);
    ssize_t size = send(sockfd, (void*)&dragMyInformationByName, sizeof(dragMyInformationByName),0);
    if(size == -1){
        qDebug()<< "send message error" << endl;
        return;
    }
    struct Information myInformation;


    while(size = recv(sockfd, &myInformation, sizeof(myInformation),0)){
        if(size < 0){
            qDebug()<< "send message error\n" << endl;
            return;
        }else if(size == 0){
            printf("Connection closed\n");
            return;
        }else{
            memset(tmpInformattion, 0,sizeof(tmpInformattion));
            strcpy(tmpInformattion, myInformation.account);
            if(tmpInformattion[0] == '\0'){
                break;
            }
            QString address = ":/QQ/";
            QString tmp;
            //memset(tmp, 0, sizeof(tmp));
            address.append(myInformation.phaddr);
            address.append(tmp);
            address.append(".jpg");
            QPixmap userpixmap(address);
            userpixmap.load(address);
            ui->userpicture->setPixmap(userpixmap);
            ui->userpicture->setScaledContents(true);

            ui->username->setText(myInformation.account);
            ui->usersignature->setText(myInformation.autograph);


        }
    }
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
    ui->friendListWidget->clear(); //清除项
    ui->blockListWidget->clear();

    int x = photoAddress.count();
    for(int i=0; i<x; i++)
    {
        if(group.at(i) == '1'){
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
            TextLabel->setObjectName(QStringLiteral("nameLabel"));
            TypeLabel->setObjectName(QStringLiteral("photoLabel"));
            QFont nameFont;
            nameFont.setFamily("微软雅黑");
            nameFont.setPointSize(12);
            TextLabel->setFont(nameFont);

            QLabel *TotalCopyLabel = new QLabel(frontArea);
            if(state.at(i) == '1'){
                TotalCopyLabel->setText("Online");
            }else{
                TotalCopyLabel->setText("Offline");
            }
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
        }else{
            QWidget *widget = new QWidget(ui->blockListWidget);
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
            TextLabel->setObjectName(QStringLiteral("nameLabel"));
            TypeLabel->setObjectName(QStringLiteral("photoLabel"));
            QFont nameFont;
            nameFont.setFamily("微软雅黑");
            nameFont.setPointSize(12);
            TextLabel->setFont(nameFont);

            QLabel *TotalCopyLabel = new QLabel(frontArea);
            if(state.at(i) == '1'){
                TotalCopyLabel->setText("Online");
            }else{
                TotalCopyLabel->setText("Offline");
            }
            QVBoxLayout *verLayout = new QVBoxLayout;
            verLayout->setContentsMargins(0, 0, 0, 0);
            verLayout->setMargin(0);
            verLayout->setSpacing(0);
            verLayout->addWidget(TextLabel);
            verLayout->addWidget(TotalCopyLabel);
            frontArea->setLayout(verLayout);
            QHBoxLayout *horLayout = new QHBoxLayout;
            horLayout->setContentsMargins(0, 0, 0, 0);
            horLayout->setMargin(0);    QTime current_time = QTime::currentTime();
            QString str;
            str.append(current_time.toString("(hh:mm:ss.zzz)"));
            horLayout->setSpacing(0);
            horLayout->addWidget(TypeLabel);
            horLayout->addWidget(frontArea);
            widget->setLayout(horLayout);
            QListWidgetItem *ITEM = new QListWidgetItem();
            QSize size = ITEM->sizeHint();
            ITEM->setSizeHint(QSize(size.width(), 56));
            ui->blockListWidget->addItem(ITEM);
            widget->setSizeIncrement(size.width(), 90);
            ui->blockListWidget->setItemWidget(ITEM, widget);
        }
    }
    QObject::connect(ui->friendListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleclicked(QListWidgetItem*)));
    QObject::connect(ui->blockListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleclicked(QListWidgetItem*)));




}



void MainWindow::showMyTalkBox(QString talkTo, QString photoAddr, QString signature){
    QStringList note = {0};
    //int x = ui->friendListWidget->currentRow();
    TalkBox *talkbox =  new TalkBox(talkTo, photoAddr,signature);
    talkbox->show();
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
        case QMessageBox::Ok:{

            //TODO: 1 means friendlist, 0 measn blocklist
            QWidget *fwid = ui->friendListWidget->itemWidget(item);
            QWidget *bwid = ui->blockListWidget->itemWidget(item);
            QString talkerPhotoAddress;
            QString talkerSig;
            QString talker;
            QString talkerPhotoAddress_temp;
            QString talkerSig_temp;
            if(bwid != NULL){
                QLabel *label = bwid->findChild<QLabel *>("nameLabel");
                talker = label->text();
            }
            else{
                QLabel *label = fwid->findChild<QLabel *>("nameLabel");
                qDebug()<<label->text()<<endl;
                talker = label->text();
            }

            //drag photo and sigfrom server
            char dragByName[40] = {0};
            strcat(dragByName, "6");
            strcat(dragByName, "|");
            strcat(dragByName, talker.toLatin1());
            ssize_t size = send(sockfd,(void*)&dragByName,sizeof(dragByName),0);
            if(size == -1){
                qDebug()<< "send message error" << endl;
                return;
            }
            struct TalkerInfo talkerWindowInfo;
            while(size = recv(sockfd, &talkerWindowInfo, sizeof(talkerWindowInfo), 0)){
                if(size < 0){
                    qDebug()<<"send msg error"<<endl;
                    return;
                }else if(size == 0){
                    //connection problem
                }else{
                    if(talkerWindowInfo.talkerPhotoAddr[0] == '\0'){
                        break;
                    }
                    talkerPhotoAddress = QString(QLatin1String(talkerWindowInfo.talkerPhotoAddr));
                    talkerSig = QString(QLatin1String(talkerWindowInfo.talkeSig));
                    break;

                }
            }
            if(fwid != NULL){
                showMyTalkBox(talker, talkerPhotoAddress, talkerSig);
            }else if(bwid != NULL){
                showMyTalkBox(talker,talkerPhotoAddress, talkerSig);
            }
            break;
        }

        default:
            qDebug()<<"nothing";
            break;
        }
    return;
}



void MainWindow::closeEvent(QCloseEvent *event){
    if(QMessageBox::question(this, tr("Quit"), tr("Are you sure to quit this application?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        char quitAsk[40] = {0};
        strcat(quitAsk, "5");
        strcat(quitAsk, "|");
        strcat(quitAsk, my_id);
        ssize_t size = send(sockfd,(void*)&quitAsk,sizeof(quitAsk),0);
        if(size == -1){
            qDebug()<< "send message error" << endl;
            return;
        }
        char quitResult[10];
        memset(quitResult, 0,sizeof(quitResult));
            while(size = recv(sockfd, &quitResult, sizeof(quitResult),0)){
                if(size < 0){
                    qDebug()<< "send message error\n" << endl;
                    return;
                }else if(size == 0){
                    printf("Connection closed\n");
                    return;
                }else{
                    if(quitResult[0] == 'q'){
                    qDebug()<<"quit successfully!"<<endl;
                    break;
                    }
                }
            }
        event->accept();
        qApp->quit();
    }else{
        event->ignore();
    }
}




MainWindow::~MainWindow()
{
    qDebug()<<"end program"<<endl;
    delete ui;
}

