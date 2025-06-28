#include "window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Window dlg;
    dlg.show();

    return app.exec();
}
