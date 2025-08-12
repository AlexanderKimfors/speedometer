#include "window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    COMservice service *
        QApplication app(argc, argv);

    Window window;
    window.show(); // visar dialogfönstret

    return app.exec();
}
