#include "window.h"

Window::Window() : canvas(width, height)
{
    mainLayout.addWidget(&canvas);
    setLayout(&mainLayout);
    setWindowTitle("Client");
    setFixedSize(width, height);
    setStyleSheet("background-color: rgb(61, 36, 53);");
}