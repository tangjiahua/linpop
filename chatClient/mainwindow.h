#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int sockfd, int my_id, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int sockfd;
    int my_id;
};

#endif // MAINWINDOW_H
