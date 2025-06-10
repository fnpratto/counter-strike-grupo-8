#ifndef CREATE_GAME_WINDOW
#define CREATE_GAME_WINDOW

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <string>

#include "common/message.h"
#include "common/queue.h"

class CreateGameWindow: public QWidget {
    Q_OBJECT

private:
    Queue<Message>& input_queue;
    Queue<Message>& output_queue;
    std::string player_name;
    QVBoxLayout* main_layout;
    QLineEdit* game_name_input;

public:
    explicit CreateGameWindow(Queue<Message>& input_queue, Queue<Message>& output_queue,
                              const std::string& player_name, QWidget* parent = nullptr);

    ~CreateGameWindow();

private:
    void init_gui();
    void add_game_title();
    void add_window_subtitle();
    void add_map_list();
    void add_game_name_input();
    void add_create_button();
    void on_create_button_clicked();
};

#endif  // CREATE_GAME_WINDOW
