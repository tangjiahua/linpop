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
#include "mulistitemdata.h""
#include "mulitemdelegate.h""

const QStringList icons = {
    ":/QQ/1.jpg", ":/QQ/2.jpeg", ":/QQ/5.jpg",
    ":/QQ/4.jpeg", ":/QQ/3.jpg", ":/QQ/6.jpeg",
};
const QStringList singers = {
    "MC-Hotdog", "李荣浩", "Author",
    "PACT", "杨千嬅", "张震岳",
};


MainWindow::MainWindow(int sockfd, int my_id , QWidget *parent) :
    sockfd(sockfd),
    my_id(my_id),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    setGeometry(1500,0,266,675);
    ui->setupUi(this);
    QStandardItemModel *pModel = new QStandardItemModel();
        for (int i=0; i<icons.size(); ++i) {
            QStandardItem *pItem = new QStandardItem;
            MyMuItemData itemData;
            itemData.name = singers.at(i);
            itemData.status = "Online";
            itemData.iconPath = icons.at(i);
            pItem->setData(QVariant::fromValue(itemData), Qt::UserRole+1);
            pModel->appendRow(pItem);
        }

        MuItemDelegate *pItemDelegate = new MuItemDelegate(this);
        ui->listView->setItemDelegate(pItemDelegate);
        ui->listView->setModel(pModel);
        connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showMyTalkBox()));

}

void MainWindow::showMyTalkBox(){
    myTalkBox = new TalkBox();
    myTalkBox->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
