#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    // Test purposes only
    Client c("185.253.218.180", 9995);
    c.setupThis();
    std::cout << c.get("Hello there!") << std::endl;
    //

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
