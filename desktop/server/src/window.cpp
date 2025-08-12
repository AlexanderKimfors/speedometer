#include "window.h"
#include "setting.h"
#include <QDebug>

window::window()
{
    // Align labels and value labels
    speedLabel.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    tempLabel.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    batteryLabel.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    speedValueLabel.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    tempValueLabel.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    batteryValueLabel.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    int maxLabelWidth = qMax(speedLabel.sizeHint().width(),
                             qMax(tempLabel.sizeHint().width(), batteryLabel.sizeHint().width()));

    speedLabel.setFixedWidth(maxLabelWidth);
    tempLabel.setFixedWidth(maxLabelWidth);
    batteryLabel.setFixedWidth(maxLabelWidth);
    lightSignalsLabel.setFixedWidth(maxLabelWidth);

    auto setupSlider = [](QSlider &slider, int min, int max)
    {
        slider.setOrientation(Qt::Horizontal);
        slider.setRange(min, max);
        slider.setValue(min);
        slider.setFixedWidth(600);
    };

    Settings::Signal &settings = Settings::Signal::getInstance();
    setupSlider(speedSlider, settings["speed"].min, settings["speed"].max);
    setupSlider(tempSlider, settings["temperature"].min, settings["temperature"].max);
    setupSlider(batterySlider, settings["battery"].min, settings["battery"].max);

    // Speed row
    speedLayout.addWidget(&speedLabel);
    speedLayout.addWidget(&speedSlider);
    speedLayout.addWidget(&speedValueLabel);
    layout.addLayout(&speedLayout);

    // Temperature row
    tempLayout.addWidget(&tempLabel);
    tempLayout.addWidget(&tempSlider);
    tempLayout.addWidget(&tempValueLabel);
    layout.addLayout(&tempLayout);

    // Battery row
    batteryLayout.addWidget(&batteryLabel);
    batteryLayout.addWidget(&batterySlider);
    batteryLayout.addWidget(&batteryValueLabel);
    layout.addLayout(&batteryLayout);

    // Light signals row
    lightSignalsLayout.addWidget(&lightSignalsLabel);
    lightSignalsLayout.addWidget(&leftCheckBox);
    lightSignalsLayout.addWidget(&rightCheckBox);
    lightSignalsLayout.addWidget(&warningCheckBox);
    layout.addLayout(&lightSignalsLayout);

    setLayout(&layout);
    setWindowTitle("Server");
    setFixedSize(800, 150);

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
