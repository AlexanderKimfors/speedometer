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
    /**
     * @brief Draws the longest lines that represents km for: 0, 20, 40, 60 .. 240
     *
     */
    void drawSpeedometerLongLines(void);
    /**
     * @brief Draws the medium lines that represents km for: 10, 30, 50, 70 .. 230
     *
     */
    void drawSpeedometerMediumLines(void);
    /**
     * @brief Draws the small lines that represents km for: 5, 15, 25, 35, .. 235
     *
     */
    void drawSpeedometerSmallLines(void);

public:
    Canvas();
};

#endif