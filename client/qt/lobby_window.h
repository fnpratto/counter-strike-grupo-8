#ifndef LOBBY_WINDOW_H
#define LOBBY_WINDOW_H

#include <QLineEdit>
#include <QString>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "common/message.h"
#include "common/queue.h"

#include "create_game_window.h"
#include "game_list_table.h"

class LobbyWindow: public QWidget {
    Q_OBJECT
private:
    Queue<Message>& input_queue;
    Queue<Message>& output_queue;
    QVBoxLayout* main_layout;
    GameListTable* game_list_table;
    QLineEdit* player_name_input;
    CreateGameWindow* create_game_window;

public:
    explicit LobbyWindow(Queue<Message>& input_queue, Queue<Message>& output_queue,
                         QWidget* parent = nullptr);
    ~LobbyWindow();

    void join_game(QString game_name);

private:
    void init_gui();
    void add_game_title();
    void add_window_subtitle();
    void add_game_list();
    void add_refresh_button();
    void add_player_name_input();
    void add_start_buttons();

    void on_create_button_clicked();
    void on_join_button_clicked();

    void update_game_list();

    void player_name_empty_warning();
};

#endif  // LOBBY_WINDOW_H
