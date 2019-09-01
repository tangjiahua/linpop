#include "talkbox.h"
#include "ui_talkbox.h"

TalkBox::TalkBox(QString addrofname, QString addrofpicture,QStringList record,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TalkBox)
{
    setGeometry(600,100,266,675);
    ui->setupUi(this);

    QPixmap p;
    p.load(addrofpicture);
    ui->talkerpicture->setPixmap(p);
    ui->talkerpicture->setScaledContents(true);
    ui->talkername->setText(addrofname);
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
