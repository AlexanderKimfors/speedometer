#ifndef WINDOW_H
#define WINDOW_H

#include "canvas.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QTimer>

class Window : public QDialog
{
    QVBoxLayout mainLayout{this};
    Canvas canvas;
    QTimer timer;

public:
    explicit Window(COMService *_service);
};

#endif // WINDOW_H
