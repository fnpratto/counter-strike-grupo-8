#ifndef LOG_IN_WINDOW_H
#define LOG_IN_WINDOW_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "game_menu.h"

#define WINDOW_WIDTH 416
#define WINDOW_HEIGHT 184

class LogInWindow: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* main_layout;
    QLineEdit* ip_input;
    QLineEdit* port_input;
    GameMenu* game_menu;

public:
    explicit LogInWindow(QWidget* parent = nullptr);
    ~LogInWindow();

private:
    void center_window();
    void init_gui();
    void add_game_title();
    void add_window_subtitle();
    void add_ip_input();
    void add_port_input();
    void add_login_button();
    void on_login_button_clicked();
};

#endif  // LOG_IN_WINDOW_H
