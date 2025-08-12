#include "window.h"
#include "tcpservice.h"
#include <QApplication>

int main(int argc, char **argv)
{
    COMService *service{TCPService{}};

    QApplication app(argc, argv);

    Window window;
    window.show(); // visar dialogfönstret

    return app.exec();
}
