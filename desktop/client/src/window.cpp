#include "window.h"

Window::Window()
{
    mainLayout.addWidget(&canvas);
    setLayout(&mainLayout);
    setWindowTitle("Client");
}