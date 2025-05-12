#ifndef GAME_LIST_TABLE_H
#define GAME_LIST_TABLE_H

#include <QTableWidget>

class GameListTable: public QTableWidget {
    Q_OBJECT
public:
    explicit GameListTable(QWidget* parent = nullptr);
    ~GameListTable() {}
};

#endif  // GAME_LIST_TABLE_H
