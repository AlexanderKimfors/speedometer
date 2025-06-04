#ifndef CANVAS_H
#define CANVAS_H

#pragma once
#include <QWidget>

class Canvas : public QWidget
{

public:
    explicit Canvas(QChar icon, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QChar icon_;
};

#endif