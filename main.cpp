#include "mainwindow.h"
#include <QApplication>

#include <cstdlib>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(time(NULL));
    MainWindow w;

    return a.exec();
}
