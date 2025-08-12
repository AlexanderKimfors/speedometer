#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>

class window : public QDialog
{

public:
    window();
    virtual ~window();

private:

    QSlider speedSlider;
    QSlider tempSlider;
    QSlider batterySlider;

    QLabel speedLabel{"Speed:"};
    QLabel speedValueLabel{"0 Km/h"};

    QLabel tempLabel{"Temperature:"};
    QLabel tempValueLabel{"-60 °C"};

    QLabel batteryLabel{"Battery Level:"};
    QLabel batteryValueLabel{"0 %"};

    QLabel lightSignalsLabel{"Light Signals:"};
    QCheckBox leftCheckBox{"Left"};
    QCheckBox rightCheckBox{"Right"};
    QCheckBox warningCheckBox{"Warning"};

    QVBoxLayout layout;

    QHBoxLayout speedLayout;
    QHBoxLayout tempLayout;
    QHBoxLayout batteryLayout;
    QHBoxLayout lightSignalsLayout;

    void onSpeedChanged(int val);
    void onTemperatureChanged(int val);
    void onBatteryChanged(int val);

    void onLeftChecked(bool checked);
    void onRightChecked(bool checked);
};

#endif // WINDOW_H
