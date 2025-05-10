#ifndef LOG_IN_WINDOW_H
#define LOG_IN_WINDOW_H

#include <QWidget>

class LogInWindow: public Qwidget {
    Q_OBJECT
private:
    QVBoxLayout* main_layout;

public:
    explicit LogInWindow(QWidget* parent = nullptr);
    ~LogInWindow();

private:
    void init_gui();
    void add_game_title();
    void add_ip_input();
    void add_port_input();
    void add_login_button();
}

#endif  // LOG_IN_WINDOW_H
