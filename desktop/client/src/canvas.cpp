#include "canvas.h"

Canvas::Canvas()
{
    // Setup default pen and brush
    pen.setColor(Qt::white);
    pen.setWidth(2);
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);

    text_font.setPointSize(12);
    icon_font.setPointSize(40);

    setMinimumSize(800, 560); // Match your window size
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setFont(text_font);
    painter.setPen(pen);
    painter.setBrush(brush);

    draw_speed();
    draw_light();
    draw_temperature();
    draw_battery_level();

    painter.end();
}

void Canvas::draw_speed()
{
    const int radius = 340;
    const int thickness = 10;
    const int center_x = width() / 2 - 50;
    const int center_y = height() - 180; // position near bottom

    QRectF outer_rect(center_x - radius, center_y - radius,
                      2 * radius, 2 * radius);

    // Set pen for arc thickness and color
    QPen arc_pen(Qt::white, thickness);
    painter.setPen(arc_pen);
    painter.setBrush(Qt::NoBrush);

    // Speed range (0–100 mapped to angle 0 to 180 degrees)
    // int angle_start = 0 * 16;  // 0 degrees
    int angle_span = 300 * 16; // 180 degrees

    // Draw background arc
    painter.drawArc(outer_rect, 240 * 16, -angle_span);

    // Draw filled arc up to current speed
    float speed_percent = qMin(speed / 100.0, 1.0); // Clamp between 0-1
    int filled_span = -angle_span * speed_percent;

    arc_pen.setColor(Qt::green);
    painter.setPen(arc_pen);
    painter.drawArc(outer_rect, 180 * 16, filled_span);

    // Draw speed value in center
    painter.setFont(text_font);
    painter.setPen(pen);
    painter.drawText(QRect(center_x - 50, center_y - 100, 100, 50),
                     Qt::AlignCenter, QString("%1 km/h").arg(speed));

    // Draw tick marks and labels (inside the arc)
    painter.setPen(Qt::white);
    painter.setFont(text_font);

    const int tick_length = 10;
    const int label_radius = radius - thickness - 20; // bring labels inside the arc
    const int tick_radius_outer = radius - thickness / 2;
    const int tick_radius_inner = tick_radius_outer - tick_length;

    for (int i = 0; i <= 12; ++i)
    {
        int speed_value = i * 20;
        float angle_deg = 205 - (300.0 / 12.0) * i; // from 240 to -60 degrees
        float angle_rad = qDegreesToRadians(angle_deg);

        // Tick line
        QPointF outer(
            center_x + tick_radius_outer * std::cos(angle_rad),
            center_y - tick_radius_outer * std::sin(angle_rad));
        QPointF inner(
            center_x + tick_radius_inner * std::cos(angle_rad),
            center_y - tick_radius_inner * std::sin(angle_rad));
        painter.drawLine(inner, outer);

        // Label position inside arc
        QPointF label_point(
            center_x + label_radius * std::cos(angle_rad) - 15,
            center_y - label_radius * std::sin(angle_rad) + 5);
        painter.drawText(QRectF(label_point, QSizeF(30, 15)),
                         Qt::AlignCenter, QString::number(speed_value));
    }
}

void Canvas::draw_light()
{
    icon_font.setPointSize(60);
    painter.setFont(icon_font);

    const int icon_width = 80;
    const int icon_height = 80;
    const int y = 30; // Vertical position of lights

    // Left light
    if (left_light)
    {

        QRect left_rect(40, y, icon_width, icon_height);
        painter.setPen(Qt::green);
        painter.drawText(left_rect, Qt::AlignCenter, QChar(0xe5c4)); // arrow_left
    }

    // Right light
    if (right_light)
    {
        QRect right_rect(590, y, icon_width, icon_height);
        painter.setPen(Qt::green);
        painter.drawText(right_rect, Qt::AlignCenter, QChar(0xe5c8)); // arrow_right
    }
}

void Canvas::draw_temperature()
{
    icon_font.setPointSize(50);
    painter.setFont(icon_font);

    const int icon_width = 100;
    const int icon_height = 60;
    const int text_height = 30;
    const int padding = 10;

    int x = width() - icon_width - padding;
    int y = height() - icon_height - text_height - padding;

    QRect icon_rect(x, y, icon_width, icon_height);
    QRect text_rect(x, y + icon_height, icon_width, text_height);

    // Set icon color based on temperature
    QColor icon_color;
    if (temperature < 5)
    {

        icon_color = Qt::white;
    }
    else if (temperature < 40)
    {

        icon_color = Qt::blue;
    }
    else
    {

        icon_color = Qt::red;
    }

    // Draw the icon with color
    painter.setPen(icon_color);
    painter.drawText(icon_rect, Qt::AlignCenter, QChar(0xe1ff)); // icon

    // Draw the temperature text with default pen
    painter.setFont(text_font);
    painter.setPen(pen); // reset to default pen before drawing text
    painter.drawText(text_rect, Qt::AlignCenter, QString("%1°C").arg(temperature));
}

void Canvas::draw_battery_level()
{
    icon_font.setPointSize(100);
    painter.setFont(icon_font);

    const int icon_width = 100;
    const int icon_height = 100;

    int x = width() - icon_width - 10;
    int y = 310;

    QRect icon_rect(x, y, icon_width, icon_height);
    QRect text_rect(x, y + icon_height - 10, icon_width, 40); // Battery % text

    // Draw full icon in default color
    painter.setPen(pen);
    painter.drawText(icon_rect, Qt::AlignCenter, QChar(0xe1a3)); // Battery icon outline

    // Determine fill color
    QColor fill_color;
    if (battery_level < 25)
    {
        fill_color = Qt::red;
    }
    else if (battery_level < 50)
    {
        fill_color = Qt::yellow;
    }
    else
    {
        fill_color = Qt::green;
    }

    // Clip area for fill
    int fill_height = icon_height * battery_level / 100;
    QRect fill_rect(icon_rect.left(), icon_rect.bottom() - fill_height, icon_width, fill_height);

    painter.setClipRect(fill_rect);                              // Clip only the fill area
    painter.setPen(fill_color);                                  // Colored pen
    painter.drawText(icon_rect, Qt::AlignCenter, QChar(0xe1a3)); // Colored battery icon fill
    painter.setClipping(false);                                  // Reset clip

    // Draw percentage text
    painter.setFont(text_font);
    painter.setPen(pen);
    painter.drawText(text_rect, Qt::AlignCenter, QString("%1%").arg(battery_level));
}

// #include "canvas.h"

// Canvas::Canvas()
// {
//     // Setup default pen and brush
//     pen.setColor(Qt::white);
//     pen.setWidth(2);
//     brush.setColor(Qt::black);
//     brush.setStyle(Qt::SolidPattern);

//     text_font.setPointSize(12);
//     icon_font.setPointSize(40);

//     // Media setup (disabled if not needed)
//     // media_player.setAudioOutput(&audio_output);

//     // Optional: set fixed size or size policy
//     setMinimumSize(200, 200);
// }

// void Canvas::paintEvent(QPaintEvent *event)
// {
//     Q_UNUSED(event);

//     painter.begin(this);
//     painter.setRenderHint(QPainter::Antialiasing);

//     painter.setFont(text_font);
//     painter.setPen(pen);
//     painter.setBrush(brush);

//     draw_speed();
//     draw_light();
//     draw_temperature();
//     draw_battery_level();

//     painter.end();
// }

// void Canvas::draw_speed()
// {
//     QRect speed_rect(10, 10, 80, 30);
//     painter.drawText(speed_rect, Qt::AlignLeft, QString("Speed: %1").arg(speed));
// }

// void Canvas::draw_light()
// {
//     QRect left_rect(10, 50, 100, 30);
//     QRect right_rect(110, 50, 100, 30);

//     painter.setPen(left_light ? Qt::yellow : Qt::gray);
//     painter.drawText(left_rect, Qt::AlignLeft, "Left Light");

//     painter.setPen(right_light ? Qt::yellow : Qt::gray);
//     painter.drawText(right_rect, Qt::AlignLeft, "Right Light");

//     painter.setPen(pen); // reset
// }

// void Canvas::draw_temperature()
// {
//     QRect temp_rect(10, 90, 120, 30);
//     painter.drawText(temp_rect, Qt::AlignLeft, QString("Temp: %1°C").arg(temperature));
// }

// void Canvas::draw_battery_level()
// {
//     QRect batt_rect(10, 130, 150, 30);
//     QString text = QString("Battery: %1%").arg(battery_level);

//     painter.drawText(batt_rect, Qt::AlignLeft, text);

//     // Battery bar
//     QRect bar_outer(170, 135, 60, 20);
//     QRect bar_inner(170, 135, battery_level * 0.6, 20); // Scale to 60px width

//     painter.drawRect(bar_outer);
//     painter.fillRect(bar_inner, Qt::green);
// }
