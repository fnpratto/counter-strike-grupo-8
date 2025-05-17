#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <QString>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "game_list_table.h"

class GameMenu: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* main_layout;
    GameListTable* game_list_table;

public:
    explicit GameMenu(QWidget* parent = nullptr);
    ~GameMenu();

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
