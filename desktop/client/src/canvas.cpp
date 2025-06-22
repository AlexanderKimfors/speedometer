#include "canvas.h"

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    const int padding{20};

    //-------------------------------------Temperature-----------------------------------------
    const int temp_width{55};
    const int temp_icon_height{60};
    const int temp_text_height{20};
    int temp_x = width() - temp_width - padding;
    int temp_y = height() - temp_icon_height - temp_text_height - padding;

    QRect temp_icon_rect(temp_x, temp_y, temp_width, temp_icon_height);
    QRect temp_text_rect(temp_x, temp_y + temp_icon_height, temp_width, temp_text_height);

    drawTemperature(temp_icon_rect, temp_text_rect);

    //------------------------------------------------------------------------------------------

    //----------------------------------------Battery-------------------------------------------

    const int battery_width{temp_width};
    const int battery_icon_height{110};
    const int battery_text_height{20};
    int battery_x{temp_x};
    int battery_y{temp_y - padding - battery_text_height - battery_icon_height};

    QRect battery_icon_rect(battery_x, battery_y, battery_width, battery_icon_height);
    QRect battery_text_rect(battery_x, battery_y + battery_icon_height, battery_width, battery_text_height);

    drawBattery(battery_icon_rect, battery_text_rect);

    //------------------------------------------------------------------------------------------
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

    painter.setClipRect(fill_rect);                              // Clip only the fill area
    painter.setPen(fill_color);                                  // Colored pen
    painter.drawText(icon_rect, Qt::AlignCenter, QChar(0xe1a3)); // Colored battery icon fill
    painter.setClipping(false);                                  // Reset clip

    text_font.setPointSize(13);
    painter.setPen("white");
    painter.setFont(text_font);
    painter.drawText(text_rect, Qt::AlignCenter, QString("%1 %").arg(battery));

    painter.end();
}