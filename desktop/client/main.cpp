#include "window.h"
//  #include "tcpservice.h"
#include <QApplication>
#include "uartservice.h"

int main(int argc, char **argv)
{
    UARTService uart;

    COMService *service{&uart};

    QApplication app(argc, argv);

    Window window{service};
    window.show(); // visar dialogfönstret

    return app.exec();
}
