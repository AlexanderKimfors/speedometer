#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "canvas.h"
class Window : public QDialog
{
    QVBoxLayout mainLayout{this};

    QHBoxLayout outer_right_side_layout;
    QVBoxLayout right_side_layout;

    Canvas battery_icon{QChar(0xebdc), 100};
    Canvas temperature_icon{QChar(0xe1ff), 50};

public:
    explicit Window(QWidget *parent = nullptr);
};
#endif // WINDOW_H

// Canvas temperature_icon{QChar(0xe1ff)};
// Canvas light_signal_left{QChar(0xe5c4)};
// Canvas light_signal_right{QChar(0xe5c8)};
// Canvas speed_icon{QChar(0xe9e4)};
// Canvas connection_error_icon{QChar(0xe628)};