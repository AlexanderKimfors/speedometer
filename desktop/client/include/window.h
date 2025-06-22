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
    QVBoxLayout mainLayout;
    Canvas canvas;

public:
};

#endif