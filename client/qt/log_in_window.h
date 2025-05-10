#ifndef LOG_IN_WINDOW_H
#define LOG_IN_WINDOW_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

// #define FONT_PATH ":/resources/counter_strike.ttf"
// #define ICON_PATH ":/resources/cs2d.bmp"
#define FONT_PATH "assets/gfx/fonts/counter_strike.ttf"
#define ICON_PATH "assets/gfx/cs2d.bmp"

#define WINDOW_WIDTH 416
#define WINDOW_HEIGHT 150
#define WINDOW_TITLE "Counter Strike 2D"

class LogInWindow: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* main_layout;
    QLineEdit* ip_input;
    QLineEdit* port_input;

public:
    LogInWindow();
    ~LogInWindow();

private:
    void init_gui();
    void add_game_title();
    void add_ip_input();
    void add_port_input();
    void add_login_button();
    void on_login_button_clicked();
};

#endif  // LOG_IN_WINDOW_H
