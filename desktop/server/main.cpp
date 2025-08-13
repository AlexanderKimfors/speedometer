#include "window.h"
#include <QApplication>
#include "tcpservice.h"

int main(int argc, char **argv)
{
    TCPService service;

    QApplication app(argc, argv);

    window dlg{&service};
    dlg.show();

    return app.exec();
}
