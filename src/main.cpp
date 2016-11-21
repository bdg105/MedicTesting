#include "winstarttesting.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinStartTesting w;
    w.show();

    return a.exec();
}
