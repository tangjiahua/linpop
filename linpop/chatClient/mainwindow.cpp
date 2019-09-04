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
#include "recvthread.h"
#include "QPicture"
#include"addfriend.h"
QStandardItemModel *pModel = new QStandardItemModel();


struct TalkerInfo{
    char talkerPhotoAddr[2];
    char talkeSig[50];
};



//char tmpInformattion[2];




QStringList friendNameList;
QStringList photoAddressList;
QStringList stateList;
QStringList groupList;

//char friendNameCurrent[20] = {0};
//char photoAddressCurrent[2] = {0};
//char stateCurrent[2] = {0};
//char groupCurrent[2] = {0};



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

    rthread = new recvThread(sockfd,this);
    rthread->start();


    dragMyInformation();
//    dragListFromServer();


    connect(rthread,SIGNAL(signal2(char*)),this,SLOT(receive2(char*)));
    connect(rthread,SIGNAL(signal3(char * )),this,SLOT(receive3(char * )));
    connect(rthread,SIGNAL(signal4(char*)),this,SLOT(receive4(char*)));
    connect(rthread,SIGNAL(signal7(char*)),this,SLOT(receive7(char*)));
    connect(rthread,SIGNAL(signala(char*)),this,SLOT(receivea(char*)));
    connect(rthread,SIGNAL(signalb(char*)),this,SLOT(receiveb(char*)));

    connect(ui->friendListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleclicked(QListWidgetItem*)));
    connect(ui->blockListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(singleclickedBlock(QListWidgetItem*)));
//    sleep(5);
//        setListView();

    connect(ui->btn_add,SIGNAL(clicked()),this,SLOT(addTo()));
}

void MainWindow::addTo()
{
    char username[10]={"tom"};
    addfriend *af = new addfriend(sockfd,username,this);
    connect(af,SIGNAL(addSucceeded()),this,SLOT(on_refreshButton_clicked()));
    af->show();
}

void MainWindow::dragMyInformation(){

    strcat(dragMyInformationByName, "3");
    strcat(dragMyInformationByName, "|");
    strcat(dragMyInformationByName, my_id);
    ssize_t size = send(sockfd, (void*)&dragMyInformationByName, sizeof(dragMyInformationByName),0);
    qDebug()<<"send dragMyInformationByName = "<<dragMyInformationByName<<endl;
    if(size == -1){
        qDebug()<< "send message error" << endl;
        return;
    }

}




void MainWindow::dragListFromServer(){
    //int sockfd, int my_id
    char dragByName[40] = {0};
    strcat(dragByName, "2");
    strcat(dragByName, "|");
    strcat(dragByName, my_id);

    ssize_t size = send(sockfd,(void*)&dragByName,sizeof(dragByName),0);
       qDebug()<<"send dragByName = "<<dragByName<<endl;
    if(size == -1){
        qDebug()<< "send message error" << endl;
        return;
    }
}


void MainWindow::setListView(){
    ui->refreshButton->setText("refresh");
    QListWidgetItem *aItem; //每一行是一个QListWidgetItem
    ui->friendListWidget->clear(); //清除项
    ui->blockListWidget->clear();

    int x = photoAddressList.count();
    for(int i=0; i<x; i++)
    {
        if(groupList.at(i) == '1'){
            QWidget *widget = new QWidget(ui->friendListWidget);
            //创建自定义的item
            widget->setStyleSheet("background:transparent;");
            QLabel *TypeLabel = new QLabel(widget);
            QString address = ":/QQ/";
            QString tmp = photoAddressList.at(i);
            address.append(tmp);
            address.append(".jpg");

            TypeLabel->setFixedSize(62, 62);
            QPixmap mypixmap;
            mypixmap.load(address);



            TypeLabel->setPixmap(mypixmap);
            TypeLabel->setScaledContents(true);
            TypeLabel->setStyleSheet("QLabel{padding-left:5px;padding-right:10px;padding-top:5px;padding-bottom:10px}");
            QWidget *frontArea = new QWidget(widget);
            frontArea->setFixedHeight(45);
            QLabel *TextLabel = new QLabel(frontArea);

            TextLabel->setText(friendNameList.at(i));//设置好友名字
            TextLabel->setObjectName(QStringLiteral("nameLabel"));
            TypeLabel->setObjectName(QStringLiteral("photoLabel"));
            QFont nameFont;
            nameFont.setFamily("微软雅黑");
            nameFont.setPointSize(12);
            TextLabel->setFont(nameFont);

            QLabel *TotalCopyLabel = new QLabel(frontArea);
            if(stateList.at(i) == '1'){
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
            QString tmp = photoAddressList.at(i);
            address.append(tmp);
            address.append(".jpg");
//            QPixmap myPix(address);
            //QPicture mypicture;
            //mypicture.load(address);

            QPixmap mypixmap;
            mypixmap.load(address);



            TypeLabel->setPixmap(mypixmap);
            TypeLabel->setFixedSize(62, 62);

            TypeLabel->setScaledContents(true);
            TypeLabel->setStyleSheet("QLabel{padding-left:5px;padding-right:10px;padding-top:5px;padding-bottom:10px}");
            QWidget *frontArea = new QWidget(widget);
            frontArea->setFixedHeight(45);
            QLabel *TextLabel = new QLabel(frontArea);

            TextLabel->setText(friendNameList.at(i));//设置好友名字
            TextLabel->setObjectName(QStringLiteral("nameLabel"));
            TypeLabel->setObjectName(QStringLiteral("photoLabel"));


            QFont nameFont;
            nameFont.setFamily("微软雅黑");
            nameFont.setPointSize(12);
            TextLabel->setFont(nameFont);

            QLabel *TotalCopyLabel = new QLabel(frontArea);
            if(stateList.at(i) == '1'){
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

}



void MainWindow::showMyTalkBox(int sockfd, QString myAccount, QString talkTo){
    //QStringList note = {0};
    //int x = ui->friendListWidget->currentRow();

    map<QString,TalkBox*>::iterator iter;
    iter = mp.find(talkTo);
    //iter = mp.find(talkTo);
    map<QString,TalkBox*> *pmp;
    pmp = &mp;
    if(iter==mp.end()){
         TalkBox *talkbox =  new TalkBox(sockfd,myAccount,talkTo,pmp,this);
         mp.insert(pair<QString,TalkBox*>(talkTo,talkbox));
         talkbox->show();
    }
    else{
        iter->second->show();
        iter->second->activateWindow();
    }

    for(int i = 0; i < ui->blockListWidget->count(); i++){
        QListWidgetItem *item = ui->blockListWidget->itemAt(0, i);
        QWidget *bwid = ui->blockListWidget->itemWidget(item);
        QLabel *label = bwid->findChild<QLabel *>("nameLabel");

//        qDebug()<< label->text() <<endl;
//        qDebug()<< talkTo<<endl;
        if(label->text().compare(talkTo) == 0){
            qDebug()<<"find some one i need !"<<endl;
            label->setStyleSheet("color:#000000;");
            break;
        }
    }

    for(int i = 0; i < ui->friendListWidget->count(); i++){
        QListWidgetItem *item = ui->friendListWidget->itemAt(0, i);
        QWidget *fwid = ui->friendListWidget->itemWidget(item);
        QLabel *flabel = fwid->findChild<QLabel *>("nameLabel");

//        qDebug()<< flabel->text() <<endl;
//        qDebug()<< talkTo<<endl;
        if(flabel->text().compare(talkTo) == 0){
            qDebug()<<"find some one i need !"<<endl;
            flabel->setStyleSheet("color:#000000;");
            break;
        }
    }
//    TalkBox *talkbox =  new TalkBox(sockfd,myAccount , talkTo);
//    talkbox->show();
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


        QStringList record={"1","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4"};

        int x = box.exec();
        QWidget *fwid = ui->friendListWidget->itemWidget(item);
        QWidget *bwid = ui->blockListWidget->itemWidget(item);
        QString talkerPhotoAddress;
        QString talker;


        if(bwid != NULL){
            QLabel *label = bwid->findChild<QLabel *>("nameLabel");
            //QLabel *plabel = bwid->findChild<QLabel *>("photoLabel");
            //plabel->picture()

            talker = label->text();
        }
        else{
            QLabel *label = fwid->findChild<QLabel *>("nameLabel");
            //QLabel *plabel = bwid->findChild<QLabel *>("photoLabel");


            qDebug()<<label->text()<<endl;
            talker = label->text();
        }





        switch (x) {
        case QMessageBox::Ok:{



            if(fwid != NULL){
                QString myid = QString(QLatin1String(my_id));
                showMyTalkBox(sockfd, myid, talker);
            }else if(bwid != NULL){
                QString myid = QString(QLatin1String(my_id));
                showMyTalkBox(sockfd, myid, talker);
            }
            break;
        }
        case QMessageBox::Cancel:{
            if(fwid != NULL){

                char* ptr;
                char *ptrmyname;
                QByteArray ba;
                    //QString str = ui->namelineEdit->text();
                ba = talker.toLatin1();
                ptr = ba.data();

                QByteArray ba_myname;
                        //QString str = ui->namelineEdit->text();
                ba_myname = ui->username->text().toLatin1();
                ptrmyname = ba_myname.data();


                char buf[10] = {0};
                strcat(buf, "a|");
                strcat(buf, ptrmyname);
                strcat(buf, "|");
                strcat(buf, ptr);
                send(sockfd, buf, sizeof(buf), 0);
                break;
            }else if(bwid != NULL){
                char* ptr;
                char* ptrmyname;
                    QByteArray ba;
                    //QString str = ui->namelineEdit->text();
                    ba = talker.toLatin1();
                    ptr = ba.data();
//                    char buf[10] = {0};
//                    strcat(buf, "a|");
//                    strcat(buf, ptr);
//                    send(sockfd, buf, sizeof(buf), 0);

                    QByteArray ba_myname;
                            //QString str = ui->namelineEdit->text();
                    ba_myname = ui->username->text().toLatin1();
                    ptrmyname = ba_myname.data();

                    char buf1[10] = {0};
                    strcat(buf1, "a|");
                    strcat(buf1, ptrmyname);
                    strcat(buf1, "|");
                    strcat(buf1, ptr);
                    send(sockfd, buf1, sizeof(buf1), 0);
                    break;
            }

        }

        case QMessageBox::No:{
            if(fwid != NULL){

                char* ptr;
                char *ptrmyname;
                QByteArray ba1;
                    //QString str = ui->namelineEdit->text();
                ba1 = talker.toLatin1();
                ptr = ba1.data();

                QByteArray ba_myname1;
                        //QString str = ui->namelineEdit->text();
                ba_myname1 = ui->username->text().toLatin1();
                ptrmyname = ba_myname1.data();


                char buf[10] = {0};
                strcat(buf, "b|");
                strcat(buf, ptrmyname);
                strcat(buf, "|");
                strcat(buf, ptr);
                send(sockfd, buf, sizeof(buf), 0);
                break;
            }else if(bwid != NULL){
                char* ptr;
                char* ptrmyname;
                    QByteArray ba1;
                    //QString str = ui->namelineEdit->text();
                    ba1 = talker.toLatin1();
                    ptr = ba1.data();
//                    char buf[10] = {0};
//                    strcat(buf, "a|");
//                    strcat(buf, ptr);
//                    send(sockfd, buf, sizeof(buf), 0);

                    QByteArray ba_myname1;
                            //QString str = ui->namelineEdit->text();
                    ba_myname1 = ui->username->text().toLatin1();
                    ptrmyname = ba_myname1.data();

                    char buf1[10] = {0};
                    strcat(buf1, "b|");
                    strcat(buf1, ptrmyname);
                    strcat(buf1, "|");
                    strcat(buf1, ptr);
                    send(sockfd, buf1, sizeof(buf1), 0);
                    break;
            }

        }
        default:
            qDebug()<<"nothing";
            break;
        }
    return;
}

void MainWindow::singleclickedBlock(QListWidgetItem*item)

{

    QMessageBox box(QMessageBox::Warning,"Choose","Choose What To Do");
        box.setWindowFlags(Qt::FramelessWindowHint);

        box.setStandardButtons(QMessageBox::Cancel|QMessageBox::Close);
        box.setButtonText(QMessageBox::Cancel, QString("Delete"));

        QCursor *mouse;
        QPoint position;
        position.setX(mouse->pos().x()-140);
        position.setY(mouse->pos().y()-30);
        box.move(QPoint(position));


        QStringList record={"1","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4","2","3","4"};

        int x = box.exec();
        QWidget *fwid = ui->friendListWidget->itemWidget(item);
        QWidget *bwid = ui->blockListWidget->itemWidget(item);
        QString talkerPhotoAddress;
        QString talker;


        if(bwid != NULL){
            QLabel *label = bwid->findChild<QLabel *>("nameLabel");
            //QLabel *plabel = bwid->findChild<QLabel *>("photoLabel");
            //plabel->picture()

            talker = label->text();
        }
        else{
            QLabel *label = fwid->findChild<QLabel *>("nameLabel");
            //QLabel *plabel = bwid->findChild<QLabel *>("photoLabel");


            qDebug()<<label->text()<<endl;
            talker = label->text();
        }





        switch (x) {

        case QMessageBox::Cancel:{
            if(fwid != NULL){

                char* ptr;
                char *ptrmyname;
                QByteArray ba;
                    //QString str = ui->namelineEdit->text();
                ba = talker.toLatin1();
                ptr = ba.data();

                QByteArray ba_myname;
                        //QString str = ui->namelineEdit->text();
                ba_myname = ui->username->text().toLatin1();
                ptrmyname = ba_myname.data();


                char buf[10] = {0};
                strcat(buf, "a|");
                strcat(buf, ptrmyname);
                strcat(buf, "|");
                strcat(buf, ptr);
                send(sockfd, buf, sizeof(buf), 0);
                break;
            }else if(bwid != NULL){
                char* ptr;
                char* ptrmyname;
                    QByteArray ba;
                    //QString str = ui->namelineEdit->text();
                    ba = talker.toLatin1();
                    ptr = ba.data();
//                    char buf[10] = {0};
//                    strcat(buf, "a|");
//                    strcat(buf, ptr);
//                    send(sockfd, buf, sizeof(buf), 0);

                    QByteArray ba_myname;
                            //QString str = ui->namelineEdit->text();
                    ba_myname = ui->username->text().toLatin1();
                    ptrmyname = ba_myname.data();

                    char buf1[10] = {0};
                    strcat(buf1, "a|");
                    strcat(buf1, ptrmyname);
                    strcat(buf1, "|");
                    strcat(buf1, ptr);
                    send(sockfd, buf1, sizeof(buf1), 0);
                    break;
            }

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

        rthread->mainwindowclosed = true;
        qDebug()<<"start quit"<<endl;

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

void MainWindow::receive_msg(char *message){

}

void MainWindow::receive3(char *message){

    qDebug()<<"receive3 "<<message<<endl;

    message+=2;
    char *account;
    char *phaddr;
    char *autograph;
    char *state;

//    sscanf(message+2, "%[^|]|%[^|]|%[^|]|%[^|]",account, phaddr,autograph, state);
    char dep[] = "|";
    account = strtok(message,dep);
    phaddr = strtok(nullptr,dep);
    autograph = strtok(nullptr,dep);
    state = strtok(nullptr,dep);

//    memset(account,0,sizeof(account));
//    memset(phaddr,0,sizeof(phaddr));
//    memset(autograph,0,sizeof(autograph));
//    memset(state,0,sizeof(state));
    qDebug()<<account<<endl;
    qDebug()<<phaddr<<endl;
    qDebug()<<autograph<<endl;
    qDebug()<<state<<endl;


//                memset(tmpInformattion, 0,sizeof(tmpInformattion));
//                strcpy(tmpInformattion, account);
                if(state[0] == '\0'){
                    return;
                }
                QString address = ":/QQ/";
//                QString tmp;
                //memset(tmp, 0, sizeof(tmp));
                address.append(phaddr);
//                address.append(tmp);
                address.append(".jpg");
                QPixmap userpixmap(address);
                userpixmap.load(address);
                ui->userpicture->setPixmap(userpixmap);
                ui->userpicture->setScaledContents(true);

                ui->username->setText(account);
                ui->usersignature->setText(autograph);
            return;
}

void MainWindow::receive2(char *message){
    qDebug()<<"receive2 "<<message<<endl;
    message+=2;
    char *account;
    char *phaddr;
    char *state;
    char *group;
    char dep[] = "|";
    account = strtok(message,dep);
    phaddr = strtok(nullptr,dep);
    state = strtok(nullptr,dep);
    group = strtok(nullptr,dep);
    qDebug()<<account<<endl;
    qDebug()<<phaddr<<endl;
    qDebug()<<state<<endl;
    qDebug()<<group<<endl;
//    sscanf(message+2, "%[^|]|%[^|]|%[^|]|%[^|]",account, phaddr, state, group);

//                memset(friendNameCurrent, 0,sizeof(friendNameCurrent));
//                memset(photoAddressCurrent, 0,sizeof(photoAddressCurrent));
//                memset(groupCurrent, 0,sizeof(groupCurrent));
//                memset(stateCurrent, 0,sizeof(stateCurrent));

//                strcpy(friendNameCurrent, account);
//                strcpy(photoAddressCurrent, phaddr);
//                strcpy(groupCurrent, group);
//                strcpy(stateCurrent, state);






                if(group[0] == '\0'){
                    return;
                }
//                photoAddressList << (phaddr);
                photoAddressList.append(phaddr);
                friendNameList.append(account);
                groupList.append(group);
                stateList.append(state);

//                qDebug()<<"friend"<<friendNameList.at(1)<<endl;
//                setListView();
                return;

}


void MainWindow::on_refreshButton_clicked()
{
    ui->refreshButton->setText("refreshing...");

    friendNameList.clear();
    photoAddressList.clear();

    stateList.clear();
    groupList.clear();

    QString uNameq = ui->username->text();
    char uName[20] = {0};
    QByteArray c= uNameq.toLocal8Bit();
    strcpy(uName, c.data());
    char msg[25] = {0};
    strcat(msg, "2|");
    strcat(msg, uName);
    send(sockfd, msg, sizeof(msg), 0);
    connect(rthread,SIGNAL(signalx(char*)),this,SLOT(setListView()));
}

void MainWindow::receive7(char *message){

    qDebug()<<"receive7 "<<message<<endl;
    message+=2;
    char *uName;
    char *fName;
    char *sendDate;
    char *sendMessage;
//    sscanf(message+2, "%[^|]|%[^|]|%[^|]|%[^|]", uName, fName, sendDate, sendMessage);
    char dep[] = "|";
    uName = strtok(message,dep);
    fName = strtok(nullptr,dep);
    sendDate = strtok(nullptr,dep);
    sendMessage = strtok(nullptr,dep);


    map<QString,TalkBox*>::iterator iter;
    iter = mp.find(uName);

    if(iter==mp.end()){
        qDebug()<<"unfind"<<endl;
        for(int i = 0; i < ui->blockListWidget->count(); i++){
            QListWidgetItem *item = ui->blockListWidget->itemAt(0, i);
            QWidget *bwid = ui->blockListWidget->itemWidget(item);
            QLabel *label = bwid->findChild<QLabel *>("nameLabel");
            QString a = QString(QLatin1String(uName));
            qDebug()<< label->text() <<endl;
            qDebug()<< a<<endl;
            if(label->text().compare(a) == 0){
                label->setStyleSheet("color:red;");
                break;
            }
        }
        for(int i = 0; i < ui->friendListWidget->count(); i++){
            QListWidgetItem *item = ui->friendListWidget->itemAt(0, i);
            QWidget *fwid = ui->friendListWidget->itemWidget(item);
            QLabel *flabel = fwid->findChild<QLabel *>("nameLabel");
            QString a;
            a = QString(QLatin1String(uName));
            qDebug()<< flabel->text() <<endl;
            qDebug()<< a<<endl;
            if(flabel->text().compare(a) == 0){
                flabel->setStyleSheet("color:red;");
                break;
            }
        }
    }
    else{
        qDebug()<<"finded"<<endl;
        iter->second->show();
        iter->second->activateWindow();
        qDebug()<<"receiveChatMsg"<<endl;
        emit receiveChatMsg(uName,fName,sendDate,sendMessage);
    }

}

void MainWindow::receivea(char *message){
    if(message[2]=='1'){
        on_refreshButton_clicked();
    }
}
void MainWindow::receiveb(char *message){
    if(message[2]=='1'){
        qDebug()<<"block and refreshed"<<endl;
        on_refreshButton_clicked();
    }
}

void MainWindow::receive4(char *message){
    char result = message[2];
    emit sendAddResult(result);
}
