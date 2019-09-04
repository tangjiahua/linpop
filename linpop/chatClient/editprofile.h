#ifndef EDITPROFILE_H
#define EDITPROFILE_H

#include <QMainWindow>

namespace Ui {
class editprofile;
}

class editprofile : public QMainWindow
{
    Q_OBJECT

public:
    explicit editprofile(QWidget *parent = 0);
    ~editprofile();

private:
    Ui::editprofile *ui;
};

#endif // EDITPROFILE_H
