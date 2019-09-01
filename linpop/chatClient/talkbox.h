#ifndef TALKBOX_H
#define TALKBOX_H

#include <QMainWindow>

namespace Ui {
class TalkBox;
}

class TalkBox : public QMainWindow
{
    Q_OBJECT

public:
    explicit TalkBox(QString addrofname, QString addrofpicture,QString signature,QStringList record,QWidget *parent = 0);
    ~TalkBox();
    QStringList talkboxrecord;
    int recordno;
    //int recordno;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::TalkBox *ui;
};

#endif // TALKBOX_H
