#include "talkbox.h"
#include "ui_talkbox.h"

TalkBox::TalkBox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TalkBox)
{
    ui->setupUi(this);
}

TalkBox::~TalkBox()
{
    delete ui;
}
