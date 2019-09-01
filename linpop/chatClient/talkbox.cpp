#include "talkbox.h"
#include "ui_talkbox.h"

TalkBox::TalkBox(QString addrofname, QString addrofpicture,QString record,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TalkBox)
{
    setGeometry(600,100,266,675);
    ui->setupUi(this);

    QString phoAdd;
    phoAdd.append(":/QQ/");
    phoAdd.append(addrofpicture);
    phoAdd.append(".jpg");
    QPixmap p;
    p.load(phoAdd);
    ui->talkerpicture->setPixmap(p);
    ui->talkerpicture->setScaledContents(true);
    ui->talkername->setText(addrofname);
    ui->talkersignature->setText(record);
}

TalkBox::~TalkBox()
{
    delete ui;
}

void TalkBox::on_pushButton_2_clicked()
{
    ui->textEdit->append(ui->lineEdit->toPlainText());
    ui->lineEdit->clear();
}
