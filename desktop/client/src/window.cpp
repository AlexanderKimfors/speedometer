#include "window.h"
#include "canvas.h"
Window::Window(COMService *_service, QWidget *parent) : canvas{_service}
    : QDialog(parent)
{
    setFixedSize(800, 560);
    setStyleSheet("background-color: rgb(61, 36, 53);");
    setLayout(&mainLayout);
    setWindowTitle("Client");
    mainLayout.addWidget(&canvas);
    canvas.set_temperature(39);
    canvas.set_battery_level(5);
    canvas.set_speed(20);
    canvas.set_light(true, true);
}
