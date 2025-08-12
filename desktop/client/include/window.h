#ifndef WINDOW_H
#define WINDOW_H

#include "canvas.h"
#include <QDialog>
#include <QVBoxLayout>

class Window : public QDialog
{
    QVBoxLayout mainLayout{this};
    Canvas canvas;

public:
    explicit Window(COMService *_service, QWidget *parent = nullptr);
};

#endif // WINDOW_H
