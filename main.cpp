#include "smapplication.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SMApplication app;
    app.showWindow();
    return a.exec();
    return 0;
}
