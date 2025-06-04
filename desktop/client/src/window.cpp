#include "window.h"
// #include "setting.h"
#include "canvas.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

Window::Window(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *row1 = new QHBoxLayout;
    QHBoxLayout *row2 = new QHBoxLayout;

    row1->addWidget(new Canvas(QChar(0xebdc)));
    row1->addWidget(new Canvas(QChar(0xe1ff)));
    row1->addWidget(new Canvas(QChar(0xe5c4)));

    row2->addWidget(new Canvas(QChar(0xe5c8)));
    row2->addWidget(new Canvas(QChar(0xe9e4)));
    row2->addWidget(new Canvas(QChar(0xe628)));

    mainLayout->addLayout(row1);
    mainLayout->addLayout(row2);

    setLayout(mainLayout);
    setWindowTitle("Material Icons Demo");
}