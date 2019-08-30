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
    explicit TalkBox(QWidget *parent = 0);
    ~TalkBox();

private:
    Ui::TalkBox *ui;
};

#endif // TALKBOX_H
