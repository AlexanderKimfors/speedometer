#include "window.h"
// #include "setting.h"
#include <QDebug>
#include "config.h"

Window::Window()
{
    int maxLabelWidth = qMax(speedLabel.sizeHint().width(),
                             qMax(tempLabel.sizeHint().width(), batteryLabel.sizeHint().width()));

    setUpSpeed(maxLabelWidth);
    setUpTemperature(maxLabelWidth);
    setUpBattery(maxLabelWidth);
    setUpLightSignals(maxLabelWidth);

    setLayout(&layout);
    setWindowTitle("Server");
    setFixedSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

    // setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
}

void Window::setUpSpeed(int width)
{

    speedLabel.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    speedLabel.setFixedWidth(width);
    speedLayout.addWidget(&speedLabel);
    layout.addLayout(&speedLayout);

    speedSlider.setOrientation(Qt::Horizontal);
    speedSlider.setRange(0, 240);
    speedSlider.setValue(0);
    speedSlider.setFixedWidth(600);
    speedLayout.addWidget(&speedSlider);

    speedValueLabel.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    speedLayout.addWidget(&speedValueLabel);

    connect(&speedSlider, &QSlider::valueChanged, this, &Window::onSpeedChanged);
}

void Window::setUpTemperature(int width)
{
    tempLabel.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tempLabel.setFixedWidth(width);
    tempLayout.addWidget(&tempLabel);
    layout.addLayout(&tempLayout);

    tempSlider.setOrientation(Qt::Horizontal);
    tempSlider.setRange(-60, 60);
    tempSlider.setValue(0);
    tempSlider.setFixedWidth(600);
    tempLayout.addWidget(&tempSlider);

    tempValueLabel.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tempLayout.addWidget(&tempValueLabel);

    connect(&tempSlider, &QSlider::valueChanged, this, &Window::onTemperatureChanged);
}

void Window::setUpBattery(int width)
{
    batteryLabel.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    batteryLabel.setFixedWidth(width);
    batteryLayout.addWidget(&batteryLabel);
    layout.addLayout(&batteryLayout);

    batterySlider.setOrientation(Qt::Horizontal);
    batterySlider.setRange(0, 100);
    batterySlider.setValue(0);
    batterySlider.setFixedWidth(600);
    batteryLayout.addWidget(&batterySlider);

    batteryValueLabel.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    batteryLayout.addWidget(&batteryValueLabel);

    connect(&batterySlider, &QSlider::valueChanged, this, &Window::onBatteryChanged);
}

void Window::setUpLightSignals(int width)
{
    lightSignalsLabel.setFixedWidth(width);
    lightSignalsLayout.addWidget(&lightSignalsLabel);

    lightSignalsLayout.addWidget(&leftCheckBox);
    lightSignalsLayout.addWidget(&rightCheckBox);

    layout.addLayout(&lightSignalsLayout);

    connect(&leftCheckBox, &QCheckBox::toggled, this, &Window::onLeftChecked);
    connect(&rightCheckBox, &QCheckBox::toggled, this, &Window::onRightChecked);
}

void Window::onSpeedChanged(int val)
{
    speedValueLabel.setText(QString("%1 km/h").arg(val));
    qDebug() << "Speed changed:" << val << "km/h";
}

void Window::onTemperatureChanged(int val)
{
    tempValueLabel.setText(QString("%1 °C").arg(val));
    qDebug() << "Temperature changed:" << val << "°C";
}

void Window::onBatteryChanged(int val)
{
    batteryValueLabel.setText(QString("%1 %").arg(val));
    qDebug() << "Battery changed:" << val << "%";
}

void Window::onLeftChecked(bool checked)
{
    qDebug() << "Left signal" << (checked ? "checked" : "unchecked");

    if (checked)
    {
        rightCheckBox.blockSignals(true);
        rightCheckBox.setChecked(false);
        rightCheckBox.setEnabled(false);
        rightCheckBox.blockSignals(false);
    }
    else
    {
        rightCheckBox.setEnabled(true);
    }
}

void Window::onRightChecked(bool checked)
{

    qDebug() << "Right signal" << (checked ? "checked" : "unchecked");

    if (checked)
    {
        leftCheckBox.blockSignals(true);
        leftCheckBox.setChecked(false);
        leftCheckBox.setEnabled(false);
        leftCheckBox.blockSignals(false);
    }
    else
    {
        leftCheckBox.setEnabled(true);
    }
}

#if 0 // warning
void Window::onLeftChecked(bool checked)
{
    if (warningCheckBox.isChecked())
    {
        qDebug() << "Left signal ignored due to active warning.";

        rightCheckBox.blockSignals(true);
        rightCheckBox.setChecked(false);
        rightCheckBox.setEnabled(true);
        rightCheckBox.blockSignals(false);
        return;
    }

    qDebug() << "Left signal" << (checked ? "checked" : "unchecked");

    if (checked)
    {
        rightCheckBox.blockSignals(true);
        rightCheckBox.setChecked(false);
        rightCheckBox.setEnabled(false);
        rightCheckBox.blockSignals(false);
    }
    else
    {
        rightCheckBox.setEnabled(true);
    }
}

void window::onRightChecked(bool checked)
{
    if (warningCheckBox.isChecked())
    {
        qDebug() << "Right signal ignored due to active warning.";

        leftCheckBox.blockSignals(true);
        leftCheckBox.setChecked(false);
        leftCheckBox.setEnabled(true);
        leftCheckBox.blockSignals(false);
        return;
    }

    qDebug() << "Right signal" << (checked ? "checked" : "unchecked");

    if (checked)
    {
        leftCheckBox.blockSignals(true);
        leftCheckBox.setChecked(false);
        leftCheckBox.setEnabled(false);
        leftCheckBox.blockSignals(false);
    }
    else
    {
        leftCheckBox.setEnabled(true);
    }
}

#endif

#if 0
window::window()
{
    lightSignalsLayout.addWidget(&warningCheckBox);


    
    connect(&warningCheckBox, &QCheckBox::toggled, this, &window::onWarningChecked);
}


void window::onWarningChecked(bool checked)
{
    qDebug() << "Warning signal" << (checked ? "checked" : "unchecked");

    if (checked)
    {
        leftCheckBox.setEnabled(true);
        rightCheckBox.setEnabled(true);
    }
    else
    {
        if (leftCheckBox.isChecked())
        {
            rightCheckBox.setEnabled(false);
        }
        else if (rightCheckBox.isChecked())
        {
            leftCheckBox.setEnabled(false);
        }
        else
        {
            leftCheckBox.setEnabled(true);
            rightCheckBox.setEnabled(true);
        }
    }
}
#endif
