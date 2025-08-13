#include "window.h"
#include "tcpservice.h"
#include <QApplication>

int main(int argc, char **argv)
{
    TCPService tcp_service;

    COMService *service{&tcp_service};

    QApplication app(argc, argv);

    Window window{service};
    window.show(); // visar dialogfönstret

    return app.exec();
}
