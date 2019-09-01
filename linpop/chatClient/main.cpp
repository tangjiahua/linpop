#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication linpop(argc, argv);
    Login login_window;
    login_window.show();


    return linpop.exec();
}
