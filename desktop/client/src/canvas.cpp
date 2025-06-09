#include "canvas.h"

static void draw_center_point(QPainter &painter, int center_x, int center_y);
static void draw_arc(QPainter &painter, QPen &pen, int thickness, int angle_span, QRect &rec);
static void draw_lines(QPainter &painter, QPen &pen, int radius, int center_x, int center_y, int thickness, int tick_length);
static void draw_text(QFont &font, QPainter &painter, QPen &pen, int thickness, int radius, int center_x, int center_y);
static void draw_speed_line(QPainter &painter, QPen &pen, int thickness, int radius, int center_x, int center_y, int speed);
static void draw_speed_text_icon(QFont &text_font, QFont &icon_font, QPainter &painter, int center_x, int center_y, int speed);
static void draw_connection_error_text_icon(QFont &text_font, QFont &icon_font, QPainter &painter, int center_x, int center_y);

Canvas::Canvas()
{
    // Setup default pen and brush
    pen.setColor(Qt::white);
    pen.setWidth(2);
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);

    text_font.setPointSize(12);
    icon_font.setPointSize(40);

    setFixedSize(800, 560); // Match your window size

    connect(&blink_timer, &QTimer::timeout, this, &Canvas::toggle_blink);
    blink_timer.start(500);

    connect(&draw_timer, &QTimer::timeout, this, [this]()
            { this->update(); });
    draw_timer.start(10);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setFont(text_font);
    painter.setPen(pen);
    painter.setBrush(brush);

    draw_temperature();
    draw_battery_level();
    draw_speed();
    draw_light();

    painter.end();
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
    pen.setColor(Qt::white);
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

void Canvas::draw_speed()
{
    const int radius = 300;
    const int thickness = 10;
    const int center_x = width() / 2 - 50;
    const int center_y = height() - 200; // position near bottom
    const int angle_span = 250 * 16;
    const int tick_length = 20;

    //-----------------------------skapar boxen för--------------------------------------
    QRect outer_rect(center_x - radius, center_y - radius,
                     2 * radius, 2 * radius);
    //-----------------------------------------------------------------------------------

    //----------------------Målar den vita circeln i mitten------------------------------
    draw_center_point(painter, center_x, center_y);
    //-----------------------------------------------------------------------------------

    //-------------------------------målar bågen-----------------------------------------
    draw_arc(painter, pen, thickness, angle_span, outer_rect);
    //-----------------------------------------------------------------------------------

    // -----------------------Draw the lines in the arc----------------------------------
    draw_lines(painter, pen, radius, center_x, center_y, thickness, tick_length);
    //-----------------------------------------------------------------------------------

    // ----------------------Draw the text of the speed----------------------------------
    draw_text(text_font, painter, pen, thickness, radius, center_x, center_y);
    //-----------------------------------------------------------------------------------

    //----------------------Målar det röda sträcket för hastigheten----------------------
    draw_speed_line(painter, pen, thickness, radius, center_x, center_y, speed);
    //-----------------------------------------------------------------------------------
    if (status)
    {
        draw_speed_text_icon(text_font, icon_font, painter, center_x, center_y, speed);
    }
    else
    {
        draw_connection_error_text_icon(text_font, icon_font, painter, center_x, center_y);
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
    if (left_light && blink_state)
    {

        QRect left_rect(40, y, icon_width, icon_height);
        painter.setPen(Qt::green);
        painter.drawText(left_rect, Qt::AlignCenter, QChar(0xe5c4)); // arrow_left
    }

    // Right light
    if (right_light && blink_state)
    {
        QRect right_rect(590, y, icon_width, icon_height);
        painter.setPen(Qt::green);
        painter.drawText(right_rect, Qt::AlignCenter, QChar(0xe5c8)); // arrow_right
    }
}

void Canvas::toggle_blink()
{
    if (left_light || right_light)
    {
        blink_state = !blink_state;
    }
    else
    {
        blink_state = true; // Visa konstant om inget blinkar
    }
}

static void draw_center_point(QPainter &painter, int center_x, int center_y)
{
    const int center_circle_radius = 20;

    painter.setPen(QPen(Qt::white, 10));
    painter.setBrush(QColor(139, 0, 0)); // Dark red
    painter.drawEllipse(QPoint(center_x, center_y), center_circle_radius, center_circle_radius);
}

static void draw_arc(QPainter &painter, QPen &pen, int thickness, int angle_span, QRect &rec)
{
    // Set pen for arc thickness and color
    pen.setColor(Qt::white);
    pen.setWidth(thickness);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    // Speed range (0–100 mapped to angle 0 to 180 degrees)
    // int angle_start = 0 * 16;  // 0 degrees

    // Draw background arc
    painter.drawArc(rec, 215 * 16, -angle_span); // start 240 grader, slut -60 grader
}

static void draw_lines(QPainter &painter, QPen &pen, int radius, int center_x, int center_y, int thickness, int tick_length)
{
    pen.setWidth(thickness - 6);
    painter.setPen(pen);

    const int tick_radius_outer = radius - (thickness / 2) - 10;
    const int tick_radius_inner = tick_radius_outer - tick_length;

    int speed_value;
    float angle_deg;
    float angle_rad;

    // Paint the big lines for each 20km
    for (int i = 0; i <= 12; ++i)
    {
        speed_value = i * 20;
        angle_deg = 210 - speed_value; // from 240 to -60 degrees
        angle_rad = qDegreesToRadians(angle_deg);

        // Tick line
        QPointF outer(
            center_x + tick_radius_outer * std::cos(angle_rad),
            center_y - tick_radius_outer * std::sin(angle_rad));
        QPointF inner(
            center_x + tick_radius_inner * std::cos(angle_rad),
            center_y - tick_radius_inner * std::sin(angle_rad));
        painter.drawLine(inner, outer);
    }
    // -----------------------------------------------------------------------

    // Paint the lines for eatch 5km
    pen.setWidth(thickness - 8);
    painter.setPen(pen);

    int minor_tick_radius_inner = tick_radius_outer - 6;

    for (int i = 0; i <= 23; ++i)
    {
        speed_value = i * 10 + 5; // 5, 15, 25, ..., 235

        angle_deg = 210 - speed_value;
        angle_rad = qDegreesToRadians(angle_deg);

        QPointF outer(
            center_x + tick_radius_outer * std::cos(angle_rad),
            center_y - tick_radius_outer * std::sin(angle_rad));
        QPointF inner(
            center_x + minor_tick_radius_inner * std::cos(angle_rad),
            center_y - minor_tick_radius_inner * std::sin(angle_rad));
        painter.drawLine(inner, outer);
    }

    // -----------------------------------------------------------------------

    minor_tick_radius_inner = tick_radius_outer - 9;

    // Paint the lines for eatch 10km
    pen.setWidth(thickness - 7);
    painter.setPen(pen);
    for (int i = 0; i < 12; ++i)
    {
        int speed_value = (i * 20) + 10;
        float angle_deg = 210 - speed_value; // from 240 to -60 degrees
        float angle_rad = qDegreesToRadians(angle_deg);

        // Tick line
        QPointF outer(
            center_x + tick_radius_outer * std::cos(angle_rad),
            center_y - tick_radius_outer * std::sin(angle_rad));
        QPointF inner(
            center_x + minor_tick_radius_inner * std::cos(angle_rad),
            center_y - minor_tick_radius_inner * std::sin(angle_rad));
        painter.drawLine(inner, outer);
    }
}

static void draw_text(QFont &font, QPainter &painter, QPen &pen, int thickness, int radius, int center_x, int center_y)
{
    font.setPointSize(18);
    painter.setFont(font);
    pen.setWidth(thickness - 6);
    painter.setPen(pen);

    const int label_radius = radius - thickness - 20;

    for (int i = 0; i <= 12; ++i)
    {
        int speed_value = i * 20;
        float angle_deg = 210 - speed_value; // from 240 to -60 degrees
        float angle_rad = qDegreesToRadians(angle_deg);

        QPointF label_point(
            center_x + (label_radius - 30) * std::cos(angle_rad) - 23,
            center_y - (label_radius - 30) * std::sin(angle_rad) - 10);
        painter.drawText(QRectF(label_point, QSizeF(40, 20)),
                         Qt::AlignCenter, QString::number(speed_value));
    }
}

static void draw_speed_line(QPainter &painter, QPen &pen, int thickness, int radius, int center_x, int center_y, int speed)
{
    pen.setWidth(thickness - 6);
    painter.setPen(pen);

    const int tick_radius_outer = radius - (thickness / 2) - 10;

    int minor_tick_radius_inner = tick_radius_outer - 45;

    pen.setWidth(thickness - 7);
    pen.setColor(QColor(139, 0, 0));
    painter.setPen(pen);
    for (int i = 0; i < 12; ++i)
    {
        float angle_deg = 210 - speed; // from 240 to -60 degrees
        float angle_rad = qDegreesToRadians(angle_deg);

        QPointF outer(
            center_x + minor_tick_radius_inner * std::cos(angle_rad),
            center_y - minor_tick_radius_inner * std::sin(angle_rad));
        QPointF inner(
            center_x,
            center_y);
        painter.drawLine(inner, outer);
    }
}

static void draw_speed_text_icon(QFont &text_font, QFont &icon_font, QPainter &painter, int center_x, int center_y, int speed)
{
    painter.setFont(text_font);
    painter.setPen(Qt::white);
    QRect text_rec = QRect(center_x - 50, center_y + 130, 100, 20);
    painter.drawText(text_rec, Qt::AlignCenter, QString("%1 km/h").arg(speed));

    icon_font.setPointSize(40);
    painter.setFont(icon_font);
    QRect icon_rec = QRect(center_x - 50, center_y + 70, 100, 50);
    painter.drawText(icon_rec, Qt::AlignCenter, QChar(0xe9e4));
}

static void draw_connection_error_text_icon(QFont &text_font, QFont &icon_font, QPainter &painter, int center_x, int center_y)
{
    painter.setFont(text_font);
    painter.setPen(Qt::red);
    QRect text_rec = QRect(center_x - 100, center_y + 130, 200, 20);
    painter.drawText(text_rec, Qt::AlignCenter, QString("Connection Error"));

    icon_font.setPointSize(40);
    painter.setFont(icon_font);
    QRect icon_rec = QRect(center_x - 50, center_y + 70, 100, 50);
    painter.drawText(icon_rec, Qt::AlignCenter, QChar(0xe628));
}