#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "game_list_table.h"

#define WINDOW_WIDTH 416
#define WINDOW_HEIGHT 184

class GameMenu: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* main_layout;
    QTableWidget* game_list;

public:
    explicit GameMenu(QWidget* parent = nullptr);
    ~GameMenu();

private:
    void init_gui();
    void add_game_title();
    void add_window_subtitle();
    void add_game_list();
    void add_start_buttons();
};

#endif  // GAME_MENU_H
