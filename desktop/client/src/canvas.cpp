#include "canvas.h"
#include <QPainter>
#include <QFont>

Canvas::Canvas(QChar icon, QWidget *parent)
    : QWidget(parent), icon_(icon)
{
    setMinimumSize(120, 120); // varje ikon får sin yta
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QFont iconFont("Material Icons", 100);
    painter.setFont(iconFont);

    painter.drawText(rect(), Qt::AlignCenter, icon_);
}
