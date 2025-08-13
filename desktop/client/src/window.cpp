#include "window.h"
#include "canvas.h"

Window::Window(COMService *_service) : canvas{_service}
{
    setFixedSize(800, 560);
    setStyleSheet("background-color: rgb(61, 36, 53);");
    setLayout(&mainLayout);
    setWindowTitle("Client");
    mainLayout.addWidget(&canvas);
}
