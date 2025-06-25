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

    drawTemperature();

    drawBattery();

    drawSpeedometerCenterCirle();
    drawSpeedometerArc();
    drawSpeedometerLongLines();
    drawSpeedometerMediumLines();
    drawSpeedometerSmallLines();
}

void Canvas::drawTemperature(void)
{
    painter.begin(this);

    QRect icon_rect(Positions::TEMP_X, Positions::TEMP_Y, Positions::TEMP_WIDTH, Positions::TEMP_ICON_HEIGHT);
    QRect text_rect(Positions::TEMP_X, Positions::TEMP_Y + Positions::TEMP_ICON_HEIGHT, Positions::TEMP_WIDTH, Positions::TEMP_TEXT_HEIGHT);

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

void Canvas::drawBattery(void)
{
    painter.begin(this);

    QRect icon_rect(Positions::BATTERY_X, Positions::BATTERY_Y, Positions::BATTERY_WIDTH, Positions::BATTERY_ICON_HEIGHT);
    QRect text_rect(Positions::BATTERY_X, Positions::BATTERY_Y + Positions::BATTERY_ICON_HEIGHT, Positions::BATTERY_WIDTH, Positions::BATTERY_TEXT_HEIGHT);

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

void Canvas::drawSpeedometerCenterCirle(void)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::white, 10));
    painter.setBrush(QColor(139, 0, 0)); // Dark red
    painter.drawEllipse(QPoint(Positions::SPEEDOMETER_CENTER_X, Positions::SPEEDOMETER_CENTER_Y), Positions::SPEEDOMETER_CENTER_CIRCLE_RADIUS, Positions::SPEEDOMETER_CENTER_CIRCLE_RADIUS);

    painter.end();
}

void Canvas::drawSpeedometerArc(void)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect outer_rect(Positions::SPEEDOMETER_CENTER_X - Positions::SPEEDOMETER_ARC_RADIUS, Positions::SPEEDOMETER_CENTER_Y - Positions::SPEEDOMETER_ARC_RADIUS,
                     2 * Positions::SPEEDOMETER_ARC_RADIUS, 2 * Positions::SPEEDOMETER_ARC_RADIUS);

    painter.setPen(QPen(Qt::white, Positions::SPEEDOMETER_ARC_THICKNESS));

    painter.drawArc(outer_rect, Positions::SPEEDOMETER_ARC_START_ANGLE * 16, -(Positions::SPEEDOMETER_ARC_DEGREES_TO_DRAW * 16));

    painter.end();
}

void Canvas::drawSpeedometerLongLines(void)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::white, Positions::SPEEDOMETER_LONG_LINES_THICHNESS));

    int speed_value;
    float angle_deg;
    float angle_rad;

    for (int i = 0; i <= 12; ++i)
    {
        speed_value = i * 20;
        angle_deg = Positions::SPEEDOMETER_ARC_START_ANGLE - Positions::SPEEDOMETER_LINES_ANGLE_OFFSET - speed_value;
        angle_rad = qDegreesToRadians(angle_deg);

        // Tick line
        QPointF outer(
            Positions::SPEEDOMETER_CENTER_X + Positions::SPEEDOMETER_LINES_START_RADIUS * std::cos(angle_rad),
            Positions::SPEEDOMETER_CENTER_Y - Positions::SPEEDOMETER_LINES_START_RADIUS * std::sin(angle_rad));
        QPointF inner(
            Positions::SPEEDOMETER_CENTER_X + Positions::SPEEDOMETER_LONG_LINES_END_RADIUS * std::cos(angle_rad),
            Positions::SPEEDOMETER_CENTER_Y - Positions::SPEEDOMETER_LONG_LINES_END_RADIUS * std::sin(angle_rad));
        painter.drawLine(inner, outer);
    }

    painter.end();
}

void Canvas::drawSpeedometerMediumLines(void)
{

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::white, Positions::SPEEDOMETER_MEDIUM_LINES_THICKNESS));

    int speed_value;
    float angle_deg;
    float angle_rad;

    for (int i = 0; i < 12; ++i)
    {
        speed_value = (i * 20) + 10;
        angle_deg = Positions::SPEEDOMETER_ARC_START_ANGLE - Positions::SPEEDOMETER_LINES_ANGLE_OFFSET - speed_value; // ändra LONG till en gemensam till alla lines?
        angle_rad = qDegreesToRadians(angle_deg);

        QPointF outer(
            Positions::SPEEDOMETER_CENTER_X + Positions::SPEEDOMETER_LINES_START_RADIUS * std::cos(angle_rad),
            Positions::SPEEDOMETER_CENTER_Y - Positions::SPEEDOMETER_LINES_START_RADIUS * std::sin(angle_rad));
        QPointF inner(
            Positions::SPEEDOMETER_CENTER_X + Positions::SPEEDOMETER_MEDIUM_LINES_END_RADIUS * std::cos(angle_rad),
            Positions::SPEEDOMETER_CENTER_Y - Positions::SPEEDOMETER_MEDIUM_LINES_END_RADIUS * std::sin(angle_rad));
        painter.drawLine(inner, outer);
    }
    painter.end();
}

void Canvas::drawSpeedometerSmallLines(void)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::white, Positions::SPEEDOMETER_SMALL_LINES_THICKNESS));

    int speed_value;
    float angle_deg;
    float angle_rad;

    for (int i = 0; i <= 23; ++i)
    {
        speed_value = i * 10 + 5; // 5, 15, 25, ..., 235

        angle_deg = Positions::SPEEDOMETER_ARC_START_ANGLE - Positions::SPEEDOMETER_LINES_ANGLE_OFFSET - speed_value;
        angle_rad = qDegreesToRadians(angle_deg);

        QPointF outer(
            Positions::SPEEDOMETER_CENTER_X + Positions::SPEEDOMETER_LINES_START_RADIUS * std::cos(angle_rad),
            Positions::SPEEDOMETER_CENTER_Y - Positions::SPEEDOMETER_LINES_START_RADIUS * std::sin(angle_rad));
        QPointF inner(
            Positions::SPEEDOMETER_CENTER_X + Positions::SPEEDOMETER_SMALL_LINES_END_RADIUS * std::cos(angle_rad),
            Positions::SPEEDOMETER_CENTER_Y - Positions::SPEEDOMETER_SMALL_LINES_END_RADIUS * std::sin(angle_rad));
        painter.drawLine(inner, outer);
    }
}