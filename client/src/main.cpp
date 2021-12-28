#include <dlfcn.h>
#include <QtWidgets>
#include <iostream>
#include <QApplication>
#include <QCoreApplication>
#include <QPushButton>
#include "../include/network.hpp"
#include "../../GUI/mainwindow.hpp"

int main(int ac, char **av) //av[1] == 0 -> client calling the other client
{
    QApplication a(ac, av);

    MainWindow w;
    w.show();

    return a.exec();
}