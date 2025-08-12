#include "window.h"
#include <QApplication>
#include "tcpservice.h"
// #include "serialportservice.h"

int main(int argc, char **argv)
{
    TCPService service;

    QApplication app(argc, argv);

    Window dlg(&service);
    dlg.show();

    return app.exec();
}
