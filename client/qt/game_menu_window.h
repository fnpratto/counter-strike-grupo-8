#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <QString>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "common/message.h"
#include "common/queue.h"

#include "game_list_table.h"

class GameMenuWindow: public QWidget {
    Q_OBJECT
private:
    Queue<Message>& input_queue;
    Queue<Message>& output_queue;
    QVBoxLayout* main_layout;
    GameListTable* game_list_table;

public:
    explicit GameMenuWindow(Queue<Message>& input_queue, Queue<Message>& output_queue,
                            QWidget* parent = nullptr);
    ~GameMenuWindow();

    void join_game(QString game_name);

private:
    void init_gui();
    void add_game_title();
    void add_window_subtitle();
    void add_game_list();
    void add_start_buttons();

    void on_create_button_clicked();
    void on_join_button_clicked();
};

#endif  // GAME_MENU_H
