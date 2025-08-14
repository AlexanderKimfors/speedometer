#include "window.h"
#include "tcpservice.h"
#include <QApplication>
// #include "uartservice.h"

int main(int argc, char **argv)
{
    // UARTService service;

    TCPService service;

    COMService *com_service{&service};

    QApplication app(argc, argv);

    Window window{com_service};
    window.show(); // visar dialogfönstret

    return app.exec();
}
