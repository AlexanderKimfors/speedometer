/*
Canvas uppgift är att vara widgeten som syns innanför window.
*/
#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>

class Canvas : public QWidget
{
private:
    QPainter painter;
    QFont icon_font{"Material Icons"};
    QFont text_font{"Arial"};

    int temperature{0};
    int battery{40};

    void paintEvent(QPaintEvent *event) override;
    void drawTemperature(void);
    void drawBattery(void);
    void drawSpeedometerCenterCirle(void);
    void drawSpeedometerArc(void);

public:
    Canvas();
};

#endif