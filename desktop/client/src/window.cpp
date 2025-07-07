#include "window.h"
#include "config.h"

Window::Window(COMService *_service) : canvas{_service}
{
    mainLayout.addWidget(&canvas);
    setLayout(&mainLayout);
    setWindowTitle("Client");
    setFixedSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    setStyleSheet("background-color: rgb(61, 36, 53);");

    connect(&draw_timer, SIGNAL(timeout()), &canvas, SLOT(update()));
    draw_timer.start(20);
}