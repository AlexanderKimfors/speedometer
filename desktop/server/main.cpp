#include "window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    COMService *service{TCPService{}};

    QApplication app(argc, argv);

    window dlg{service};
    dlg.show();

    return app.exec();
}
