#include "../server/include/window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    window dlg;
    dlg.show();

    return app.exec();
}
