#include "OpencvApp.h"
#include <QtWidgets/QApplication>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QFile qss("dark\\style.qss");
    QFile qss("styleSheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        std::cout << "qss read success" << std::endl;
    }
    else {
        std::cout << "qss read fail" << std::endl;
    }
    QString style = QLatin1String(qss.readAll());
    a.setStyleSheet(style);

    OpencvApp w;
    w.show();
    return a.exec();
}
