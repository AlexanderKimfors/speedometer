#include "window.h"
#include <QApplication>
#include "comservice.h"
#include "tcpservice.h"
// #include "serialportservice.h"

int main(int argc, char **argv)
{
    TCPService service;

    QApplication app(argc, argv);

    Window window(&service);
    window.show();

    return app.exec();
}