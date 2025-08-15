#include "window.h"
#include "canvas.h"
#include "setting.h"

Window::Window(COMService *_service) : canvas{_service}
{
    setFixedSize(800, 560);
    setStyleSheet("background-color: rgb(61, 36, 53);");
    setLayout(&mainLayout);
    setWindowTitle("Client");
    mainLayout.addWidget(&canvas);
    connect(&draw_timer, SIGNAL(timeout()), &canvas, SLOT(update()));
    connect(&blink_timer, &QTimer::timeout, &canvas, &Canvas::toggle_blink);
    draw_timer.start(settings::DRAW_INTERVAL);
    blink_timer.start(settings::BLINK_INTERVAL);
}
