#ifndef GAME_LIST_TABLE_H
#define GAME_LIST_TABLE_H

#include <QTableWidget>
#include <vector>

#include "common/responses.h"

class LobbyWindow;

class GameListTable: public QTableWidget {
    Q_OBJECT
public:
    explicit GameListTable(QWidget* parent = nullptr);
    ~GameListTable() {}

    QString get_selected_game();
    void update_game_list(const std::vector<GameInfo>& game_list);

private:
    void on_cell_double_clicked(int row, int column);
};

#endif  // GAME_LIST_TABLE_H
