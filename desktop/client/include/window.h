/*
Windows uppgift är att hålla en layout som har våran canvas som widget.
*/
#ifndef WINDOW_H
#define WINDOW_H

#include "canvas.h"
#include <QDialog>
#include <QVBoxLayout>

class Window : public QDialog
{
private:
    static constexpr int width{800};
    static constexpr int height{560};

    QVBoxLayout mainLayout;
    Canvas canvas;

public:
    Window();
};

#endif