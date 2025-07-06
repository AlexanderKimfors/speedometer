#include "window.h"
#include "config.h"

Window::Window(COMService *_service) : canvas{_service}
{
    mainLayout.addWidget(&canvas);
    setLayout(&mainLayout);
    setWindowTitle("Client");
    setFixedSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    setStyleSheet("background-color: rgb(61, 36, 53);");

    connect(&timer, SIGNAL(timeout()), &canvas, SLOT(update()));

    // Start the timer with a timeout interval (e.g., 100 ms)
    timer.start(20);
}