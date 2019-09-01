#include "talkbox.h"
#include "ui_talkbox.h"
#include "qscrollbar.h"
#include "qmessagebox.h"
TalkBox::TalkBox(QString addrofname, QString addrofpicture,QString signature,QStringList record,QWidget *parent) :
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
    ui->talkersignature->setText(signature);

    talkboxrecord=record;
    recordno=talkboxrecord.count()-1;
    ui->textEdit->clear();

}

TalkBox::~TalkBox()
{
    delete ui;
}

void TalkBox::on_pushButton_2_clicked()
{
    QTextCursor talkboxcursor=ui->textEdit->textCursor();
    ui->textEdit->append(ui->lineEdit->toPlainText());

    ui->lineEdit->clear();
    QScrollBar *scrollbar = ui->textEdit->verticalScrollBar();
    scrollbar->setSliderPosition(scrollbar->maximum());
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
