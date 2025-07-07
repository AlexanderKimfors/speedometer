#include "window.h"
#include "settings.h"
#include <QDebug>
#include "config.h"

Window::Window(COMService *_service) : service{_service}
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
}

void Window::setUpSpeed(int width)
{
    speedLabel.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    speedLabel.setFixedWidth(width);
    speedLayout.addWidget(&speedLabel);
    layout.addLayout(&speedLayout);

    speedSlider.setOrientation(Qt::Horizontal);
    speedSlider.setRange(Settings::Signal::getInstance()["speed"].min, Settings::Signal::getInstance()["speed"].max);
    speedSlider.setValue(0);
    speedSlider.setFixedWidth(Config::SLIDER_WIDTH);
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
    tempSlider.setRange(Settings::Signal::getInstance()["temperature"].min, Settings::Signal::getInstance()["temperature"].max);
    tempSlider.setValue(0);
    tempSlider.setFixedWidth(Config::SLIDER_WIDTH);
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
    batterySlider.setRange(Settings::Signal::getInstance()["battery"].min, Settings::Signal::getInstance()["battery"].max);
    batterySlider.setValue(0);
    batterySlider.setFixedWidth(Config::SLIDER_WIDTH);
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
    lightSignalsLayout.addWidget(&warningCheckBox);

    layout.addLayout(&lightSignalsLayout);

    connect(&leftCheckBox, &QCheckBox::toggled, this, &Window::onLeftChecked);
    connect(&rightCheckBox, &QCheckBox::toggled, this, &Window::onRightChecked);
    connect(&warningCheckBox, &QCheckBox::toggled, this, &Window::onWarningChecked);
}

void Window::onSpeedChanged(int val)
{
    service->set_speed(val);
    speedValueLabel.setText(QString("%1 km/h").arg(val));
    qDebug() << "Speed changed:" << val << "km/h";
}

void Window::onTemperatureChanged(int val)
{
    service->set_temperature(val);
    tempValueLabel.setText(QString("%1 °C").arg(val));
    qDebug() << "Temperature changed:" << val << "°C";
}

void Window::onBatteryChanged(int val)
{
    service->set_battery(val);
    batteryValueLabel.setText(QString("%1 %").arg(val));
    qDebug() << "Battery changed:" << val << "%";
}

void Window::onLeftChecked(bool checked)
{
    qDebug() << "Left signal" << (checked ? "checked" : "unchecked");

    if (!warningCheckBox.isChecked())
    {
        service->set_left_light(checked);
    }

    if (checked)
    {

        rightCheckBox.setEnabled(false);
    }
    else
    {
        rightCheckBox.setEnabled(true);
    }
}

void Window::onRightChecked(bool checked)
{
    qDebug() << "Right signal" << (checked ? "checked" : "unchecked");

    if (!warningCheckBox.isChecked())
    {
        service->set_right_light(checked);
    }

    if (checked)
    {
        leftCheckBox.setEnabled(false);
    }
    else
    {
        leftCheckBox.setEnabled(true);
    }
}

void Window::onWarningChecked(bool checked)
{
    qDebug() << "Warning signal" << (checked ? "checked" : "unchecked");

    if (checked)
    {
        service->set_left_light(true);
        service->set_right_light(true);
    }
    else
    {
        if (leftCheckBox.isChecked())
        {
            service->set_left_light(true);
            rightCheckBox.setEnabled(false);
        }
        else
        {
            service->set_left_light(false);
            rightCheckBox.setEnabled(true);
        }
        if (rightCheckBox.isChecked())
        {
            service->set_right_light(true);
            leftCheckBox.setEnabled(false);
        }
        else
        {
            service->set_right_light(false);
            leftCheckBox.setEnabled(true);
        }
    }
}
