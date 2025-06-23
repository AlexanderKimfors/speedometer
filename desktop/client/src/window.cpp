#include "window.h"
#include "config.h"

Window::Window()
{
    mainLayout.addWidget(&canvas);
    setLayout(&mainLayout);
    setWindowTitle("Client");
    setFixedSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    setStyleSheet("background-color: rgb(61, 36, 53);");
}