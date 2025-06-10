#ifndef CANVAS_H
#define CANVAS_H

#include <QTimer>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
// #include <QAudioOutput>
// #include <QMediaPlayer>

class Canvas : public QWidget
{
    QPen pen;
    QBrush brush; // tror denna ska användas till batteriet, men vi kör just nu utan den pga att vi använder en symbol.
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
    void set_temperature(int temp) { temperature = temp; }
    void set_battery_level(int percent) { battery_level = percent; }
    void set_speed(int spd) { speed = spd; }
#if 0
    void set_status(int sttus) { status = sttus; }
#endif

private:
    int speed{0};
    int temperature{0};
    int battery_level{0};
    bool status{false};
    bool left_light{false};
    bool blink_state{true};
    bool right_light{false};
    QTimer draw_timer;
    QTimer blink_timer;

    void draw_temperature(void);
    void draw_battery_level(void);
    void draw_speed(void);
    void draw_light(void);
    void toggle_blink(void);

    void paintEvent(QPaintEvent *event) override;
};
#endif // CANVAS_H
