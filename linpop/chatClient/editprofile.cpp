#include "editprofile.h"
#include "ui_editprofile.h"

editprofile::editprofile(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::editprofile)
{
    ui->setupUi(this);
}

editprofile::~editprofile()
{
    delete ui;
}
