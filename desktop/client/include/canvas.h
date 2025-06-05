#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
// #include <QAudioOutput>
// #include <QMediaPlayer>

class Canvas : public QWidget
{
    QPen pen;
    QBrush brush;
    QPainter painter;
    // QAudioOutput audio_output;
    // QMediaPlayer media_player;
    QFont text_font{"Arial"};
    QFont icon_font{"Material Icons"};

public:
    Canvas();

    void set_light(bool left, bool right)
    {
        left_light = left;
        right_light = right;
    }

    void set_battery_level(int percent) { battery_level = percent; }
    void set_temperature(int temp) { temperature = temp; }
    void set_status(int sttus) { status = sttus; }
    void set_speed(int spd) { speed = spd; }

private:
    int speed{0};
    bool status{false};
    int temperature{0};
    int battery_level{0};
    bool left_light{false};
    bool right_light{false};

    void draw_speed(void);
    void draw_light(void);
    void draw_temperature(void);
    void draw_battery_level(void);

    void paintEvent(QPaintEvent *event) override;
};
#endif // CANVAS_H
