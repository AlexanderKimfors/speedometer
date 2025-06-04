#include "window.h"
// #include "setting.h"
#include <QDebug>

window::window()
{
    auto setupSlider = [](QSlider &slider, int min, int max)
    {
        slider.setOrientation(Qt::Horizontal);
        slider.setRange(min, max);
        slider.setValue(min);
        slider.setFixedWidth(600); 
    };

    setupSlider(speedSlider, 0, 240);
    setupSlider(tempSlider, -60, 60);
    setupSlider(batterySlider, 0, 100);

    // Speed row
    QHBoxLayout *speedLayout = new QHBoxLayout;
    speedLayout->addWidget(&speedLabel);
    speedLayout->addWidget(&speedSlider);
    speedLayout->addWidget(&speedValueLabel);
    layout.addLayout(speedLayout);

    // Temperature row
    QHBoxLayout *tempLayout = new QHBoxLayout;
    tempLayout->addWidget(&tempLabel);
    tempLayout->addWidget(&tempSlider);
    tempLayout->addWidget(&tempValueLabel);
    layout.addLayout(tempLayout);

    // Battery row
    QHBoxLayout *batteryLayout = new QHBoxLayout;
    batteryLayout->addWidget(&batteryLabel);
    batteryLayout->addWidget(&batterySlider);
    batteryLayout->addWidget(&batteryValueLabel);
    layout.addLayout(batteryLayout);

    // Light signals row
    QHBoxLayout *lightSignalsLayout = new QHBoxLayout;
    lightSignalsLayout->addWidget(&lightSignalsLabel);
    lightSignalsLayout->addWidget(&leftCheckBox);
    lightSignalsLayout->addWidget(&rightCheckBox);
    lightSignalsLayout->addWidget(&warningCheckBox);
    layout.addLayout(lightSignalsLayout);

    setLayout(&layout);
    setWindowTitle("Server");
    setFixedSize(800, 200);

    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);

    connect(&speedSlider, &QSlider::valueChanged, this, &window::onSpeedChanged);
    connect(&tempSlider, &QSlider::valueChanged, this, &window::onTemperatureChanged);
    connect(&batterySlider, &QSlider::valueChanged, this, &window::onBatteryChanged);

    connect(&leftCheckBox, &QCheckBox::toggled, this, &window::onLeftChecked);
    connect(&rightCheckBox, &QCheckBox::toggled, this, &window::onRightChecked);
    connect(&warningCheckBox, &QCheckBox::toggled, this, &window::onWarningChecked);
}

window::~window() {}

void window::onSpeedChanged(int val)
{
    speedValueLabel.setText(QString("%1 km/h").arg(val));
    qDebug() << "Speed changed:" << val << "km/h";
}

void window::onTemperatureChanged(int val)
{
    tempValueLabel.setText(QString("%1 °C").arg(val));
    qDebug() << "Temperature changed:" << val << "°C";
}

void window::onBatteryChanged(int val)
{
    batteryValueLabel.setText(QString("%1 %").arg(val));
    qDebug() << "Battery changed:" << val << "%";
}

void window::onLeftChecked(bool checked)
{
    qDebug() << "Left signal" << (checked ? "checked" : "unchecked");
    if (checked)
    {
        rightCheckBox.setChecked(false);
        rightCheckBox.setEnabled(false);
        warningCheckBox.setChecked(false);
        warningCheckBox.setEnabled(true);
    }
    else
    {
        rightCheckBox.setEnabled(true);
    }
}

void window::onRightChecked(bool checked)
{
    qDebug() << "Right signal" << (checked ? "checked" : "unchecked");
    if (checked)
    {
        leftCheckBox.setChecked(false);
        leftCheckBox.setEnabled(false);
        warningCheckBox.setChecked(false);
        warningCheckBox.setEnabled(true);
    }
    else
    {
        leftCheckBox.setEnabled(true);
    }
}

void window::onWarningChecked(bool checked)
{
    qDebug() << "Warning signal" << (checked ? "checked" : "unchecked");
    // No disabling of left or right checkboxes here
}
