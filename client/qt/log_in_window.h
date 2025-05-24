#ifndef LOG_IN_WINDOW_H
#define LOG_IN_WINDOW_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "common/message.h"
#include "common/queue.h"

#include "lobby_window.h"

class LogInWindow: public QWidget {
    Q_OBJECT
private:
    Queue<Message>& input_queue;
    Queue<Message>& output_queue;
    QVBoxLayout* main_layout;
    QLineEdit* ip_input;
    QLineEdit* port_input;
    LobbyWindow* game_menu;

public:
    explicit LogInWindow(Queue<Message>& input_queue, Queue<Message>& output_queue,
                         QWidget* parent = nullptr);
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
