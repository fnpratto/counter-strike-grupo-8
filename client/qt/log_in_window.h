#ifndef LOG_IN_WINDOW_H
#define LOG_IN_WINDOW_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

class LogInWindow: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* main_layout;
    QLineEdit* ip_input;
    QLineEdit* port_input;

public:
    explicit LogInWindow(int width, int height);
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
