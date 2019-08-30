#include "talkbox.h"
#include "ui_talkbox.h"

TalkBox::TalkBox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TalkBox)
{
    setGeometry(600,100,266,675);
    ui->setupUi(this);

}

TalkBox::~TalkBox()
{
    delete ui;
}
