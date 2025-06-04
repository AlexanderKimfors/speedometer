// #include "setting.h"
#include "window.h"
#include "canvas.h"
Window::Window(QWidget *parent)
    : QDialog(parent)
{
    right_side_layout.addWidget(&battery_icon);
    right_side_layout.addWidget(&temperature_icon);
    outer_right_side_layout.addStretch();
    outer_right_side_layout.addLayout(&right_side_layout);

    mainLayout.addStretch();
    mainLayout.addLayout(&outer_right_side_layout);

    setFixedSize(800, 560);
    setStyleSheet("background-color: rgb(61, 36, 53);");
    setLayout(&mainLayout);

    setWindowTitle("Material Icons Demo");
}

// row1.addWidget(&temperature_icon);
// row1.addWidget(&light_signal_left);

// row2.addWidget(&light_signal_right);
// row2.addWidget(&speed_icon);
// row2.addWidget(&connection_error_icon);
