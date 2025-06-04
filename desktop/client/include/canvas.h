#ifndef CANVAS_H
#define CANVAS_H

#include <QChar>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class Canvas : public QWidget
{

public:
    explicit Canvas(QChar icon, int fontSize, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QChar icon_;
    int fontSize_;
};

#endif