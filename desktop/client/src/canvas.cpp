#include "canvas.h"
#include "config.h"
#include "positions.h"

Canvas::Canvas()
{
    setFixedSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QRect temp_icon(Positions::TEMP_X, Positions::TEMP_Y, Positions::TEMP_WIDTH, Positions::TEMP_ICON_HEIGHT);
    QRect temp_text(Positions::TEMP_X, Positions::TEMP_Y + Positions::TEMP_ICON_HEIGHT, Positions::TEMP_WIDTH, Positions::TEMP_TEXT_HEIGHT);
    drawTemperature(temp_icon, temp_text);

    QRect battery_icon(Positions::BATTERY_X, Positions::BATTERY_Y, Positions::BATTERY_WIDTH, Positions::BATTERY_ICON_HEIGHT);
    QRect battery_text(Positions::BATTERY_X, Positions::BATTERY_Y + Positions::BATTERY_ICON_HEIGHT, Positions::BATTERY_WIDTH, Positions::BATTERY_TEXT_HEIGHT);
    drawBattery(battery_icon, battery_text);
}

void Canvas::drawTemperature(QRect &icon_rect, QRect &text_rect)
{
    painter.begin(this);

    QColor color;
    if (temperature < 5)
    {
        color = Qt::white;
    }
    else if (temperature < 40)
    {
        color = Qt::blue;
    }
    else
    {
        color = Qt::red;
    }

    icon_font.setPointSize(45);
    painter.setFont(icon_font);
    painter.setPen(color);
    painter.drawText(icon_rect, Qt::AlignCenter, QChar(0xe1ff));

    text_font.setPointSize(13);
    color = "white";
    painter.setPen(color);
    painter.setFont(text_font);
    painter.drawText(text_rect, Qt::AlignCenter, QString("%1 °C").arg(temperature));

    painter.end();
}

void Canvas::drawBattery(QRect &icon_rect, QRect &text_rect)
{
    painter.begin(this);

    icon_font.setPointSize(90);
    painter.setFont(icon_font);
    painter.setPen("white");
    painter.drawText(icon_rect, Qt::AlignCenter, QChar(0xe1a3));

    QColor fill_color;
    if (battery < 25)
    {
        fill_color = Qt::red;
    }
    else if (battery < 50)
    {
        fill_color = Qt::yellow;
    }
    else
    {
        fill_color = Qt::green;
    }

    // Clip area for fill
    int fill_height = icon_rect.height() * battery / 100;
    QRect fill_rect(icon_rect.left(), icon_rect.bottom() - fill_height, icon_rect.width(), fill_height);

    painter.setClipRect(fill_rect);
    painter.setPen(fill_color);
    painter.drawText(icon_rect, Qt::AlignCenter, QChar(0xe1a3));
    painter.setClipping(false);

    text_font.setPointSize(13);
    painter.setPen("white");
    painter.setFont(text_font);
    painter.drawText(text_rect, Qt::AlignCenter, QString("%1 %").arg(battery));

    painter.end();
}