#include "canvas.h"
#include <QPainter>
#include <QFont>

Canvas::Canvas(QChar icon, int fontSize, QWidget *parent)
    : QWidget(parent), icon_(icon), fontSize_(fontSize)
{
    setMinimumSize(120, 120);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QFont iconFont("Material Icons", fontSize_);
    painter.setFont(iconFont);

    painter.drawText(rect(), Qt::AlignCenter, icon_);
}
