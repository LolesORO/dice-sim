#include "testqtApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    testqtApp window;
    window.show();
    return app.exec();
}
