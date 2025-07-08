#ifndef WINDOW_H
#define WINDOW_H

#include "canvas.h"
#include <QDialog>
#include <QVBoxLayout>
#include "comservice.h"
#include <QTimer>

class Window : public QDialog
{
private:
    QVBoxLayout mainLayout;
    Canvas canvas;
    QTimer draw_timer;

public:
    Window(COMService *_service);
};

#endif